#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2Noise.hpp"
#include "VoxTerGenAlgorithms/utils/Hash.hpp"
#include "VoxTerGenAlgorithms/utils/Math.hpp"

#include <cstdint>
#include <numbers>
#include <array>
#include <cmath>

/**
 * @note This source code was adapted from Kurt Spencer's and Jordan Peck's implementations.
 * Sources: https://github.com/KdotJPG/OpenSimplex2
 *			https://github.com/Auburn/FastNoiseLite
 */

namespace
{
	constexpr long long seed_flip_3d = -0x52D547B2E96ED629LL;
	constexpr long long seed_offset_4d = 0xE83DC3E0DA7164DLL;

	constexpr double root2_over2 = 0.7071067811865476;
	constexpr double skew_2d = 0.366025403784439;
	constexpr double unskew_2d = -0.21132486540518713;

	constexpr double root3_over3 = 0.577350269189626;
	constexpr double fallback_rotate_3d = 2.0 / 3.0;
	constexpr double rotate_3d_orthogonalizer = unskew_2d;

	constexpr double skew_4d = -0.138196601125011;
	constexpr double unskew_4d = 0.309016994374947;
	constexpr double lattice_step_4d = 0.2;

	constexpr double rsquared_2d = 0.5;
	constexpr double rsquared_3d = 0.6;
	constexpr double rsquared_4d = 0.6;
}

OpenSimplex2Noise::OpenSimplex2Noise(
	std::uint64_t seed, 
	OpenSimplex2Variant variant, 
	Noise2DModifier noise_2d_modifier, 
	Noise3DModifier noise_3d_modifier)	
	: 
	seed_(seed), 
	variant_(variant), 
	noise_2d_modifier_(noise_2d_modifier), 
	noise_3d_modifier_(noise_3d_modifier), 
	noise_4d_modifier_(Noise4DModifier::Default)
{
}

double OpenSimplex2Noise::Noise(double x, double y) const noexcept
{
	double xr = x;
	double yr = y;

	RotateCoords(xr, yr);

	const std::int64_t xrb = static_cast<std::int64_t>(std::floor(xr));
	const std::int64_t yrb = static_cast<std::int64_t>(std::floor(yr));
	const double xi = static_cast<double>(xr - xrb);
	const double yi = static_cast<double>(yr - yrb);

	const std::uint64_t xsbp = hash::HashCoords(seed_, xrb);
	const std::uint64_t ysbp = hash::HashCoords(seed_, yrb);

	const double t = (xi + yi) * unskew_2d;
	const double dx0 = xi + t;
	const double dy0 = yi + t;

	double value = 0.0;
	const double a0 = rsquared_2d - dx0 * dx0 - dy0 * dy0;

	if (a0 > 0)
	{
		value = (a0 * a0) * (a0 * a0) * math::DotGrad(hash::HashCoords(seed_, xsbp, ysbp), dx0, dy0);
	}

	constexpr double k1 = 1.0 + 2.0 * unskew_2d;
	constexpr double k2 = 1.0 / unskew_2d + 2.0;

	const double a1 = (2.0 * k1 * k2) * t + (-2.0 * k1 * k1 + a0);
	
	if (a1 > 0)
	{
		const double dx1 = dx0 - static_cast<double>(1 + 2 * unskew_2d);
		const double dy1 = dy0 - static_cast<double>(1 + 2 * unskew_2d);
		value += math::Pow4(a1) * math::DotGrad(hash::HashCoords(seed_, xsbp + 1, ysbp + 1), dx1, dy1);
	}

	if (variant_ == OpenSimplex2Variant::Fast)
	{
		if (dy0 > dx0)
		{
			const double dx2 = dx0 - static_cast<double>(unskew_2d);
			const double dy2 = dy0 - static_cast<double>(unskew_2d + 1);
			const double a2 = rsquared_2d - dx2 * dx2 - dy2 * dy2;

			if (a2 > 0)
			{
				value += math::Pow4(a2) * math::DotGrad(hash::HashCoords(seed_, xsbp, ysbp + 1), dx2, dy2);
			}
		}
		else
		{
			const double dx2 = dx0 - static_cast<double>(unskew_2d + 1);
			const double dy2 = dy0 - static_cast<double>(unskew_2d);

			const double a2 = rsquared_2d - dx2 * dx2 - dy2 * dy2;

			if (a2 > 0)
			{
				value += math::Pow4(a2) * math::DotGrad(hash::HashCoords(seed_, xsbp + 1, ysbp), dx2, dy2);
			}
		}
	}
	else if (variant_ == OpenSimplex2Variant::Smooth)
	{
		const double xmyi = xi - yi;

		if (t < unskew_2d)
		{
			if (xi + xmyi > 1.0)
			{
				const double dx2 = dx0 - static_cast<double>(3 * unskew_2d + 2);
				const double dy2 = dy0 - static_cast<double>(3 * unskew_2d + 1);
				const double a2 = rsquared_2d - dx2 * dx2 - dy2 * dy2;

				if (a2 > 0)
				{
					value += math::Pow4(a2) * math::DotGrad(hash::HashCoords(seed_, xsbp + 2, ysbp + 1), dx2, dy2);
				}
			}
			else
			{
				const double dx2 = dx0 - static_cast<double>(unskew_2d);
				const double dy2 = dy0 - static_cast<double>(unskew_2d + 1);
				const double a2 = rsquared_2d - dx2 * dx2 - dy2 * dy2;

				if (a2 > 0)
				{
					value += math::Pow4(a2) * math::DotGrad(hash::HashCoords(seed_, xsbp, ysbp + 1), dx2, dy2);
				}
			}

			if (yi - xmyi > 1.0)
			{
				const double dx3 = dx0 - static_cast<double>(3 * unskew_2d + 1);
				const double dy3 = dy0 - static_cast<double>(3 * unskew_2d + 2);
				const double a3 = rsquared_2d - dx3 * dx3 - dy3 * dy3;
				
				if (a3 > 0)
				{
					value += math::Pow4(a3) * math::DotGrad(hash::HashCoords(seed_, xsbp + 1, ysbp + 2), dx3, dy3);
				}
			}
			else
			{
				const double dx3 = dx0 - static_cast<double>(unskew_2d + 1);
				const double dy3 = dy0 - static_cast<double>(unskew_2d);
				const double a3 = rsquared_2d - dx3 * dx3 - dy3 * dy3;
				
				if (a3 > 0) 
				{
					value += math::Pow4(a3) * math::DotGrad(hash::HashCoords(seed_, xsbp + 1, ysbp), dx3, dy3);
				}
			}
		}
		else
		{
			if (xi + xmyi < 0.0)
			{
				const double dx2 = dx0 + static_cast<double>(unskew_2d + 1);
				const double dy2 = dy0 + static_cast<double>(unskew_2d);
				const double a2 = rsquared_2d - dx2 * dx2 - dy2 * dy2;
				
				if (a2 > 0) 
				{
					value += math::Pow4(a2) * math::DotGrad(hash::HashCoords(seed_, xsbp - 1, ysbp), dx2, dy2);
				}
			}
			else
			{
				const double dx2 = dx0 - static_cast<double>(unskew_2d + 1);
				const double dy2 = dy0 - static_cast<double>(unskew_2d);
				const double a2 = rsquared_2d - dx2 * dx2 - dy2 * dy2;
				
				if (a2 > 0)
				{
					value += math::Pow4(a2) * math::DotGrad(hash::HashCoords(seed_, xsbp + 1, ysbp), dx2, dy2);
				}
			}

			if (yi < xmyi) 
			{
				const double dx2 = dx0 + static_cast<double>(unskew_2d);
				const double dy2 = dy0 + static_cast<double>(unskew_2d + 1);
				const double a2 = rsquared_2d - dx2 * dx2 - dy2 * dy2;
				
				if (a2 > 0)
				{
					value += math::Pow4(a2) * math::DotGrad(hash::HashCoords(seed_, xsbp, ysbp - 1), dx2, dy2);
				}
			}
			else
			{
				const double dx2 = dx0 - static_cast<double>(unskew_2d);
				const double dy2 = dy0 - static_cast<double>(unskew_2d + 1);
				const double a2 = rsquared_2d - dx2 * dx2 - dy2 * dy2;
				
				if (a2 > 0)
				{
					value += math::Pow4(a2) * math::DotGrad(hash::HashCoords(seed_, xsbp, ysbp + 1), dx2, dy2);
				}
			}
		}
	}

	return value;
}

double OpenSimplex2Noise::Noise(double x, double y, double z) const noexcept
{
	if (variant_ == OpenSimplex2Variant::Fast)
	{
		return NoiseFast(x, y, z);
	}
	else if (variant_ == OpenSimplex2Variant::Smooth)
	{
		return NoiseSmooth(x, y, z);
	}

	assert(false);
	return 0.0;
}

//double OpenSimplex2Noise::Noise(double x, double y, double z, double w) const noexcept
//{
//	double xr = x;
//	double yr = y;
//	double zr = z;
//	double wr = w;
//
//	RotateCoords(xr, yr, zr, wr);
//
//	const std::int64_t xrb = static_cast<std::int64_t>(std::round(xr));
//	const std::int64_t yrb = static_cast<std::int64_t>(std::round(yr));
//	const std::int64_t zrb = static_cast<std::int64_t>(std::round(zr));
//	const std::int64_t wrb = static_cast<std::int64_t>(std::round(wr));
//
//	const double xri = static_cast<double>(xr - xrb);
//	const double yri = static_cast<double>(yr - yrb);
//	const double zri = static_cast<double>(zr - zrb);
//	const double wri = static_cast<double>(wr - wrb);
//
//	const double siSum = (xri + yri) + (zri + wri);
//	const std::uint64_t startingLattice = static_cast<std::uint64_t>(siSum * 1.25);
//
//	const std::uint64_t offset_seed = seed_ + startingLattice * seed_offset_4d;
//
//	if (variant_ == OpenSimplex2Variant::Fast)
//	{
//
//	}
//	else if (variant_ == OpenSimplex2Variant::Smooth)
//	{
//
//	}
//
//	return 0.0;
//}

void OpenSimplex2Noise::RotateCoords(double& xr, double& yr) const noexcept
{
	if (noise_2d_modifier_ == Noise2DModifier::Skew)
	{
		const double s = skew_2d * (xr + yr);

		xr += s;
		yr += s;
	}
	else if (noise_2d_modifier_ == Noise2DModifier::ImproveX)
	{
		const double xx = xr * root2_over2;
		const double yy = yr * (root2_over2 * (1.0 + 2.0 * skew_2d));

		xr = yy + xx;
		yr = yy - xx;
	}
}

void OpenSimplex2Noise::RotateCoords(double& xr, double& yr, double& zr) const noexcept
{
	if (noise_3d_modifier_ == Noise3DModifier::ImproveXZ)
	{
		const double xz = xr + zr;
		const double s2 = xz * rotate_3d_orthogonalizer;
		const double yy = yr * root3_over3;
		
		xr = xr + s2 + yy;
		zr = zr + s2 + yy;
		yr = xz * -root3_over3 + yy;
	}
	else if (noise_3d_modifier_ == Noise3DModifier::ImproveXY)
	{
		const double xy = xr + yr;
		const double s2 = xy * rotate_3d_orthogonalizer;
		const double zz = zr * root3_over3;
		
		xr = xr + s2 + zz;
		yr = yr + s2 + zz;
		zr = xy * -root3_over3 + zz;
	}
	else if (noise_3d_modifier_ == Noise3DModifier::Fallback)
	{
		const double r = fallback_rotate_3d * (xr + yr + zr);

		xr = r - xr;
		yr = r - yr;
		zr = r - zr;
	}
}

void OpenSimplex2Noise::RotateCoords(double& xr, double& yr, double& zr, double& wr) const noexcept
{
	if (noise_4d_modifier_ == Noise4DModifier::ImproveXYZ)
	{
		const double xyz = xr + yr + zr;
        const double ww = wr * 0.2236067977499788;
        const double s2 = xyz * -0.16666666666666666 + ww;

        xr += s2;
		yr += s2;
		zr += s2;
		wr = -0.5 * xyz + ww;
	}
	else if (noise_4d_modifier_ == Noise4DModifier::ImproveXYZ_ImproveXY)
	{
		const double xy = xr + yr;
        const double s2 = xy * -0.21132486540518699998;
        const double zz = zr * 0.28867513459481294226;
        const double ww = wr * 0.2236067977499788;
        
		xr = xr + (zz + ww + s2);
		yr = yr + (zz + ww + s2);
        zr = xy * -0.57735026918962599998 + (zz + ww);
        wr = zr * -0.866025403784439 + ww;
	}
	else if (noise_4d_modifier_ == Noise4DModifier::ImproveXYZ_ImproveXZ)
	{
		const double xz = xr + zr;
        const double s2 = xz * -0.21132486540518699998;
        const double yy = yr * 0.28867513459481294226;
        const double ww = wr * 0.2236067977499788;

        xr = xr + (yy + ww + s2);
		zr = zr + (yy + ww + s2);
        yr = xz * -0.57735026918962599998 + (yy + ww);
        wr = yr * -0.866025403784439 + ww;
	}
	else if (noise_4d_modifier_ == Noise4DModifier::ImproveXY_ImproveZW)
	{
		const double s2 = (xr + yr) * -0.178275657951399372 + (zr + wr) * 0.215623393288842828;
        const double t2 = (zr + wr) * -0.403949762580207112 + (xr + yr) * -0.375199083010075342;

        xr += s2;
		yr += s2;
		zr += t2;
		wr += t2;
	}
	else if (noise_4d_modifier_ == Noise4DModifier::Fallback)
	{
		const double s = skew_4d * (xr + yr + zr + wr);

        xr += s;
		yr += s;
		zr += s;
		wr += s;
	}
}

double OpenSimplex2Noise::NoiseFast(double x, double y, double z) const noexcept
{
	std::uint64_t seed = seed_;

	double xr = x;
	double yr = y;
	double zr = z;

	RotateCoords(xr, yr, zr);

	std::int64_t xrb = static_cast<std::int64_t>(std::round(xr));
	std::int64_t yrb = static_cast<std::int64_t>(std::round(yr));
	std::int64_t zrb = static_cast<std::int64_t>(std::round(zr));

	double xri = static_cast<double>(xr - xrb);
	double yri = static_cast<double>(yr - yrb);
	double zri = static_cast<double>(zr - zrb);

	std::int64_t xNSign = (xri < 0) ? 1 : -1;
	std::int64_t yNSign = (yri < 0) ? 1 : -1;
	std::int64_t zNSign = (zri < 0) ? 1 : -1;

	double ax0 = xNSign * -xri;
	double ay0 = yNSign * -yri;
	double az0 = zNSign * -zri;

	const std::uint64_t xrbp = hash::HashCoords(seed, xrb);
	const std::uint64_t yrbp = hash::HashCoords(seed, yrb);
	const std::uint64_t zrbp = hash::HashCoords(seed, zrb);

	double value = 0.0;
	double a = (rsquared_3d - xri * xri) - (yri * yri + zri * zri);

	for (int l = 0;;l++)
	{
		if (a > 0) 
		{
			value += math::Pow4(a) * math::DotGrad(hash::HashCoords(seed, xrbp, yrbp, zrbp), xri, yri, zri);
		}

		if (ax0 >= ay0 && ax0 >= az0)
		{
			double b = a + ax0 + ax0;

			if (b > 1)
			{
				--b;
				value += math::Pow4(b) * math::DotGrad(hash::HashCoords(seed, xrbp - xNSign, yrbp, zrbp), xri + xNSign, yri, zri);
			}
		}
		else if (ay0 > ax0 && ay0 >= az0)
		{
			double b = a + ay0 + ay0;

			if (b > 1)
			{
				--b;
				value += math::Pow4(b) * math::DotGrad(hash::HashCoords(seed, xrbp, yrbp - yNSign, zrbp), xri, yri + yNSign, zri);
			}
		}
		else
		{
			double b = a + az0 + az0;
				
			if (b > 1)
			{
				--b;
				value += math::Pow4(b) * math::DotGrad(hash::HashCoords(seed, xrbp, yrbp, zrbp - zNSign), xri, yri, zri + zNSign);
			}
		}

		if (l == 1)
		{
			break;
		}

		ax0 = 0.5 - ax0;
		ay0 = 0.5 - ay0;
		az0 = 0.5 - az0;

		xri = xNSign * ax0;
		yri = yNSign * ay0;
		zri = zNSign * az0;

		a += (0.75 - ax0) - (ay0 + az0);

		xrb += (xNSign < 0) ? 1 : 0;
		yrb += (yNSign < 0) ? 1 : 0;
		zrb += (zNSign < 0) ? 1 : 0;

		xNSign = -xNSign;
		yNSign = -yNSign;
		zNSign = -zNSign;

		seed ^= seed_flip_3d;
	}

	return value;
}

double OpenSimplex2Noise::NoiseSmooth(double x, double y, double z) const noexcept
{
	std::uint64_t seed2 = seed_ ^ -0x52D547B2E96ED629LL;

	double xr = x;
	double yr = y;
	double zr = z;

	RotateCoords(xr, yr, zr);

	const std::int64_t xrb = static_cast<std::int64_t>(std::floor(xr));
	const std::int64_t yrb = static_cast<std::int64_t>(std::floor(yr));
	const std::int64_t zrb = static_cast<std::int64_t>(std::floor(zr));

	const double xi = static_cast<double>(xr - xrb);
	const double yi = static_cast<double>(yr - yrb);
	const double zi = static_cast<double>(zr - zrb);

	const std::uint64_t xrbp = hash::HashCoords(seed_, xrb);
	const std::uint64_t yrbp = hash::HashCoords(seed_, yrb);
	const std::uint64_t zrbp = hash::HashCoords(seed_, zrb);

	const int xNMask = static_cast<int>(-0.5 - xi);
	const int yNMask = static_cast<int>(-0.5 - yi);
	const int zNMask = static_cast<int>(-0.5 - zi);

	const double x0 = xi + xNMask;
	const double y0 = yi + yNMask;
	const double z0 = zi + zNMask;
	const double a0 = rsquared_3d - x0 * x0 - y0 * y0 - z0 * z0;

	const std::uint64_t hash = hash::HashCoords(seed_, xrbp + (xNMask & 1), yrbp + (yNMask & 1), zrbp + (zNMask & 1));
	double value = (a0 * a0) * (a0 * a0) * math::DotGrad(hash, x0, y0, z0);

	const double x1 = xi - 0.5;
	const double y1 = yi - 0.5;
	const double z1 = zi - 0.5;
	const double a1 = rsquared_3d - x1 * x1 - y1 * y1 - z1 * z1;
	value += (a1 * a1) * (a1 * a1) * math::DotGrad(hash::HashCoords(seed_, xrbp + 1, yrbp + 1, zrbp + 1), x1, y1, z1);

	const double xAFlipMask0 = (xNMask == 0 ? 2.0 : -2.0) * x1;
	const double yAFlipMask0 = (yNMask == 0 ? 2.0 : -2.0) * y1;
	const double zAFlipMask0 = (zNMask == 0 ? 2.0 : -2.0) * z1;
	const double xAFlipMask1 = (-2 - (xNMask << 2)) * x1 - 1.0f;
	const double yAFlipMask1 = (-2 - (yNMask << 2)) * y1 - 1.0f;
	const double zAFlipMask1 = (-2 - (zNMask << 2)) * z1 - 1.0f;
	
	auto dir = [](int mask) -> double { return (mask == 0) ? 1.0 : -1.0; };
	auto shiftNeg = [](int mask) -> int { return (mask < 0) ? 1 : 0; };
	auto shiftPos = [](int mask) -> int { return (mask == 0) ? 1 : 0; };
	auto shiftNeg2 = [](int mask) -> int { return (mask < 0) ? 2 : 0; };
	
	bool skip5 = false;
	
	double a2 = xAFlipMask0 + a0;
	
	if (a2 > 0)
	{
		double x2 = x0 - (xNMask == 0 ? 1.0 : -1.0);
		double y2 = y0;
		double z2 = z0;

		const std::uint64_t hash = hash::HashCoords(seed_, xrb + shiftPos(xNMask), yrb + shiftNeg(yNMask), zrb + shiftNeg(zNMask));
		value += math::Pow4(a2) * math::DotGrad(hash, x2, y2, z2);
	}
	else
	{
		double a3 = yAFlipMask0 + zAFlipMask0 + a0;
		
		if (a3 > 0)
		{
			double x3 = x0;
			double y3 = y0 - (yNMask == 0 ? 1.0 : -1.0);
			double z3 = z0 - (zNMask == 0 ? 1.0 : -1.0);

			const std::uint64_t hash = hash::HashCoords(seed_, xrb + shiftNeg(xNMask), yrb + shiftPos(yNMask), zrb + shiftPos(zNMask));
			value += math::Pow4(a3) * math::DotGrad(hash, x3, y3, z3);
		}

		double a4 = xAFlipMask1 + a1;
		
		if (a4 > 0)
		{
			double x4 = (xNMask == 0 ? 1.0 : -1.0) + x1;
			double y4 = y1;
			double z4 = z1;

			const std::uint64_t hash = hash::HashCoords(seed2, xrb + shiftNeg2(xNMask), yrb + 1, zrb + 1);
			value += math::Pow4(a4) * math::DotGrad(hash, x4, y4, z4);
			skip5 = true;
		}
	}


	bool skip9 = false;

	double a6 = yAFlipMask0 + a0;
	if (a6 > 0)
	{
		double x6 = x0;
		double y6 = y0 - dir(yNMask);
		double z6 = z0;
		
		const std::uint64_t hash = hash::HashCoords(seed_, xrb + shiftNeg(xNMask), yrb + shiftPos(yNMask), zrb + shiftNeg(zNMask));
		value += math::Pow4(a6) * math::DotGrad(hash, x6, y6, z6);
	}
	else
	{
		double a7 = xAFlipMask0 + zAFlipMask0 + a0;

		if (a7 > 0)
		{
			double x7 = x0 - dir(xNMask);
			double y7 = y0;
			double z7 = z0 - dir(zNMask);

			const std::uint64_t hash = hash::HashCoords(seed_, xrb + shiftPos(xNMask), yrb + shiftNeg(yNMask), zrb + shiftPos(zNMask));
			value += math::Pow4(a7) * math::DotGrad(hash, x7, y7, z7);
		}

		double a8 = yAFlipMask1 + a1;

		if (a8 > 0)
		{
			double x8 = x1;
			double y8 = dir(yNMask) + y1;
			double z8 = z1;

			const std::uint64_t hash = hash::HashCoords(seed2, xrb + 1, yrb + shiftNeg2(yNMask), zrb + 1);
			value += math::Pow4(a8) * math::DotGrad(hash, x8, y8, z8);

			skip9 = true;
		}
	}

	bool skipD = false;

	double aA = zAFlipMask0 + a0;

	if (aA > 0)
	{
		double xA = x0;
		double yA = y0;
		double zA = z0 - dir(zNMask);

		const std::uint64_t hash = hash::HashCoords(seed_, xrb + shiftNeg(xNMask), yrb + shiftNeg(yNMask), zrb + shiftPos(zNMask));
		value += math::Pow4(aA) * math::DotGrad(hash, xA, yA, zA);
	}
	else
	{
		double aB = xAFlipMask0 + yAFlipMask0 + a0;

		if (aB > 0)
		{
			double xB = x0 - dir(xNMask);
			double yB = y0 - dir(yNMask);
			double zB = z0;

			const std::uint64_t hash = hash::HashCoords(seed_, xrb + shiftPos(xNMask), yrb + shiftPos(yNMask), zrb + shiftNeg(zNMask));
			value += math::Pow4(aB) * math::DotGrad(hash, xB, yB, zB);
		}

		double aC = zAFlipMask1 + a1;

		if (aC > 0)
		{
			double xC = x1;
			double yC = y1;
			double zC = dir(zNMask) + z1;

			const std::uint64_t hash = hash::HashCoords(seed2, xrb + 1, yrb + 1, zrb + shiftNeg2(zNMask));
			value += math::Pow4(aC) * math::DotGrad(hash, xC, yC, zC);
			skipD = true;
		}
	}

	if (!skip5)
	{
		double a5 = yAFlipMask1 + zAFlipMask1 + a1;

		if (a5 > 0)
		{
			double x5 = x1;
			double y5 = dir(yNMask) + y1;
			double z5 = dir(zNMask) + z1;

			const std::uint64_t hash = hash::HashCoords(seed2, xrb + 1, yrb + shiftNeg2(yNMask), zrb + shiftNeg2(zNMask));
			value += math::Pow4(a5) * math::DotGrad(hash, x5, y5, z5);
		}
	}

	if (!skip9)
	{
		double a9 = xAFlipMask1 + zAFlipMask1 + a1;

		if (a9 > 0)
		{
			double x9 = dir(xNMask) + x1;
			double y9 = y1;
			double z9 = dir(zNMask) + z1;

			const std::uint64_t hash = hash::HashCoords(seed2, xrb + shiftNeg2(xNMask), yrb + 1, zrb + shiftNeg2(zNMask));
			value += math::Pow4(a9) * math::DotGrad(hash, x9, y9, z9);
		}
	}

	if (!skipD)
	{
		double aD = xAFlipMask1 + yAFlipMask1 + a1;

		if (aD > 0)
		{
			double xD = dir(xNMask) + x1;
			double yD = dir(yNMask) + y1;
			double zD = z1;

			const std::uint64_t hash = hash::HashCoords(seed2, xrb + shiftNeg2(xNMask), yrb + shiftNeg2(yNMask), zrb + 1);
			value += math::Pow4(aD) * math::DotGrad(hash, xD, yD, zD);
		}
	}

	return value;
}