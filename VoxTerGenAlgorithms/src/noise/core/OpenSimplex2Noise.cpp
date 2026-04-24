#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2Noise.hpp"

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
	constexpr double root2_over2 = 0.7071067811865476;
	constexpr double skew_2d = 0.366025403784439;
	constexpr double unskew_2d = -0.21132486540518713;

	constexpr double root3_over3 = 0.577350269189626;
	constexpr double fallback_rotate_3d = 2.0 / 3.0;
	constexpr double rotate_3d_orthogonalizer = unskew_2d;

	constexpr double skew_4d = -0.138196601125011;
	constexpr double unskew_4d = 0.309016994374947;
	constexpr double lattice_step_4d = 0.2;
}

OpenSimplex2Noise::OpenSimplex2Noise(
	std::uint64_t seed, 
	OpenSimplex2Variant variant, 
	Noise2DModifier noise_2d_modifier, 
	Noise3DModifier noise_3d_modifier, 
	Noise4DModifier noise_4d_modifier) 
	: 
	seed_(seed), 
	variant_(variant), 
	noise_2d_modifier_(noise_2d_modifier), 
	noise_3d_modifier_(noise_3d_modifier), 
	noise_4d_modifier_(noise_4d_modifier)
{
}

double OpenSimplex2Noise::Noise(double x, double y) const noexcept
{
	double xr = x;
	double yr = y;

	RotateCoords(xr, yr);

	if (variant_ == OpenSimplex2Variant::Fast)
	{

	}
	else if (variant_ == OpenSimplex2Variant::Smooth)
	{

	}

	return 0.0;
}

double OpenSimplex2Noise::Noise(double x, double y, double z) const noexcept
{
	double xr = x;
	double yr = y;
	double zr = z;

	RotateCoords(xr, yr, zr);

	if (variant_ == OpenSimplex2Variant::Fast)
	{

	}
	else if (variant_ == OpenSimplex2Variant::Smooth)
	{

	}

	return 0.0;
}

double OpenSimplex2Noise::Noise(double x, double y, double z, double w) const noexcept
{
	double xr = x;
	double yr = y;
	double zr = z;
	double wr = w;

	RotateCoords(xr, yr, zr, wr);

	if (variant_ == OpenSimplex2Variant::Fast)
	{

	}
	else if (variant_ == OpenSimplex2Variant::Smooth)
	{

	}

	return 0.0;
}

void OpenSimplex2Noise::RotateCoords(double& xr, double& yr) const noexcept
{
	if (noise_2d_modifier_ == Noise2DModifier::Skew || noise_2d_modifier_ == Noise2DModifier::Default)
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
	if (noise_3d_modifier_ == Noise3DModifier::ImproveXZ || noise_3d_modifier_ == Noise3DModifier::Default)
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
	if (noise_4d_modifier_ == Noise4DModifier::ImproveXYZ || noise_4d_modifier_ == Noise4DModifier::Default)
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