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
	constexpr double fallback_rotate_2d = 2.0 / 3.0;
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
	double xr = 0.0;
	double yr = 0.0;

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
	double xr = 0.0;
	double yr = 0.0;
	double zr = 0.0;

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
	// TODO

	double xr = 0.0;
	double yr = 0.0;
	double zr = 0.0;
	double wr = 0.0;

	RotateCoords(xr, yr, zr, wr);

	if (variant_ == OpenSimplex2Variant::Fast)
	{

	}
	else if (variant_ == OpenSimplex2Variant::Smooth)
	{

	}

	return 0.0;
}

void OpenSimplex2Noise::RotateCoords(double& rx, double& ry) const noexcept
{
	if (noise_2d_modifier_ == Noise2DModifier::ImproveXY)
	{

	}
	else if (noise_2d_modifier_ == Noise2DModifier::ImproveX)
	{

	}
	else if (noise_2d_modifier_ == Noise2DModifier::Fallback)
	{

	}
}

void OpenSimplex2Noise::RotateCoords(double& rx, double& ry, double& rz) const noexcept
{
	if (noise_3d_modifier_ == Noise3DModifier::ImproveXZ)
	{

	}
	else if (noise_3d_modifier_ == Noise3DModifier::ImproveXY)
	{

	}
	else if (noise_3d_modifier_ == Noise3DModifier::Fallback)
	{

	}
}

void OpenSimplex2Noise::RotateCoords(double& rx, double& ry, double& rz, double& rw) const noexcept
{
	if (noise_4d_modifier_ == Noise4DModifier::ImproveXYZ)
	{

	}
	else if (noise_4d_modifier_ == Noise4DModifier::ImproveXY)
	{

	}
	else if (noise_4d_modifier_ == Noise4DModifier::ImproveXZ)
	{

	}
	else if (noise_4d_modifier_ == Noise4DModifier::ImproveYZ)
	{

	}
	else if (noise_4d_modifier_ == Noise4DModifier::ImproveXYZW)
	{

	}
	else if (noise_4d_modifier_ == Noise4DModifier::Fallback)
	{

	}
}