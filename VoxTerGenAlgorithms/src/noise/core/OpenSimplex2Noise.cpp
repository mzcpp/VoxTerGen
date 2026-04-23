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

	Improve(xr, yr);

	return 0.0;
}

double OpenSimplex2Noise::Noise(double x, double y, double z) const noexcept
{
	double xr = 0.0;
	double yr = 0.0;
	double zr = 0.0;

	Improve(xr, yr, zr);

	return 0.0;
}

double OpenSimplex2Noise::Noise(double x, double y, double z, double w) const noexcept
{
	double xr = 0.0;
	double yr = 0.0;
	double zr = 0.0;
	double wr = 0.0;

	Improve(xr, yr, zr, wr);

	// TODO

	return 0.0;
}

void OpenSimplex2Noise::Improve(double& rx, double& ry) const noexcept
{
	if (noise_2d_modifier_ == Noise2DModifier::Default || noise_2d_modifier_ == Noise2DModifier::ImproveXY)
	{

	}
	else if (noise_2d_modifier_ == Noise2DModifier::ImproveX)
	{

	}
	else if (noise_2d_modifier_ == Noise2DModifier::Fallback)
	{

	}
}

void OpenSimplex2Noise::Improve(double& rx, double& ry, double& rz) const noexcept
{
	if (noise_3d_modifier_ == Noise3DModifier::Default || noise_3d_modifier_ == Noise3DModifier::ImproveXZ)
	{

	}
	else if (noise_3d_modifier_ == Noise3DModifier::ImproveXY)
	{

	}
	else if (noise_3d_modifier_ == Noise3DModifier::Fallback)
	{

	}
}

void OpenSimplex2Noise::Improve(double& rx, double& ry, double& rz, double& rw) const noexcept
{
	if (noise_4d_modifier_ == Noise4DModifier::Default || noise_3d_modifier_ == Noise4DModifier::ImproveXYZ)
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