#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/SimplexNoise.hpp"

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"
#include "VoxTerGenAlgorithms/utils/Hash.hpp"
#include "VoxTerGenAlgorithms/utils/Math.hpp"

#include <cstdint>
#include <numbers>
#include <array>
#include <cmath>
#include <numeric>

/**
 * @note This source code was adapted from Stefan Gustavson's paper on Ken Perlin's Simplex Noise.
 * Source: https://www.researchgate.net/publication/216813608_Simplex_noise_demystified
 */

namespace
{
	constexpr double skew_2d = 0.366025403784439;
	constexpr double unskew_2d = -0.21132486540518713;
	constexpr double skew_4d = 0.309016994374947f;
	constexpr double unskew_4d = -0.138196601125011f;
}

SimplexNoise::SimplexNoise(std::uint64_t seed) : Noise(seed)
{
}

double SimplexNoise::Sample(double x) const noexcept
{
	return 0.0;
}

double SimplexNoise::Sample(double x, double y) const noexcept
{
	// Skew the coordinate system to determine the simplex we're in.
	const double F2 = 0.5 * (std::numbers::sqrt3 - 1.0);
	const double s = (x + y) * F2;
	const ivec2 simplex_coords = { static_cast<int>(std::floor(x + s)), static_cast<int>(std::floor(y + s)) };

	// Unskew the cell origin back to axis-aligned space to determine distance from origin.
	const double G2 = (3.0 - std::numbers::sqrt3) / 6.0;
	const double t = (simplex_coords[0] + simplex_coords[1]) * G2;
	const dvec2 p0 = { x - (simplex_coords[0] - t), y - (simplex_coords[1] - t) };

	// Find the coordinates of the other 2 corners.
	const ivec2 p1_offsets = { p0[0] > p0[1], p0[0] <= p0[1] };
	const dvec2 p1 = { p0[0] - p1_offsets[0] + G2, p0[1] - p1_offsets[1] + G2 };
	const dvec2 p2 = { p0[0] - 1.0 + (2.0 * G2), p0[1] - 1.0 + (2.0 * G2) };

	const std::uint64_t p0_hash = hash::HashCoords(seed_, simplex_coords[0], simplex_coords[1]);
	const std::uint64_t p1_hash = hash::HashCoords(seed_, simplex_coords[0] + p1_offsets[0], simplex_coords[1] + p1_offsets[1]);
	const std::uint64_t p2_hash = hash::HashCoords(seed_, simplex_coords[0] + 1, simplex_coords[1] + 1);

	const double t0 = 0.5 - (p0[0] * p0[0]) - (p0[1] * p0[1]);
	const double t1 = 0.5 - (p1[0] * p1[0]) - (p1[1] * p1[1]);
	const double t2 = 0.5 - (p2[0] * p2[0]) - (p2[1] * p2[1]);

	const dvec4 corner_contributions =
	{
		(t0 < 0) ? 0.0 : math::Pow4(t0) * math::DotGrad(p0_hash, p0[0], p0[1]),
		(t1 < 0) ? 0.0 : math::Pow4(t1) * math::DotGrad(p1_hash, p1[0], p1[1]),
		(t2 < 0) ? 0.0 : math::Pow4(t2) * math::DotGrad(p2_hash, p2[0], p2[1])
	};

	return std::accumulate(corner_contributions.begin(), corner_contributions.end(), 0.0); // normalize
}

double SimplexNoise::Sample(double x, double y, double z) const noexcept
{
	// Skew the coordinate system to determine the simplex we're in.
	const double F3 = 1.0 / 3.0;
	const double s = (x + y + z) * F3;
	const ivec3 simplex_coords = { static_cast<int>(std::floor(x + s)), static_cast<int>(std::floor(y + s)), static_cast<int>(std::floor(z + s)) };

	// Unskew the cell origin back to axis-aligned space to determine distance from origin.
	const double G3 = 1.0 / 6.0;
	const double t = (simplex_coords[0] + simplex_coords[1] + simplex_coords[2]) * G3;
	const dvec3 p0 = { x - (simplex_coords[0] - t), y - (simplex_coords[1] - t), z - (simplex_coords[2] - t) };

	ivec3 p1_offsets = { 0, 0, 0 };
	ivec3 p2_offsets = { 0, 0, 0 };
	GetP2P3Offsets(p0, p1_offsets, p2_offsets);

	const dvec3 p1 = { p0[0] - p1_offsets[0] + G3, p0[1] - p1_offsets[1] + G3, p0[2] - p1_offsets[2] + G3 };
	const dvec3 p2 = { p0[0] - p2_offsets[0] + G3, p0[1] - p2_offsets[1] + G3, p0[2] - p2_offsets[2] + G3 };;
	const dvec3 p3 = { p0[0] - 1.0 + (3.0 * G3), p0[1] - 1.0 + (3.0 * G3), p0[2] - 1.0 + (3.0 * G3) };

	const std::uint64_t p0_hash = hash::HashCoords(seed_, simplex_coords[0], simplex_coords[1], simplex_coords[2]);
	const std::uint64_t p1_hash = hash::HashCoords(seed_, simplex_coords[0] + p1_offsets[0], simplex_coords[1] + p1_offsets[1], simplex_coords[2] + p1_offsets[2]);
	const std::uint64_t p2_hash = hash::HashCoords(seed_, simplex_coords[0] + p2_offsets[0], simplex_coords[1] + p2_offsets[1], simplex_coords[2] + p2_offsets[2]);
	const std::uint64_t p3_hash = hash::HashCoords(seed_, simplex_coords[0] + 1, simplex_coords[1] + 1, simplex_coords[2] + 1);

	const double t0 = 0.5 - (p0[0] * p0[0]) - (p0[1] * p0[1]) - (p0[2] * p0[2]);
	const double t1 = 0.5 - (p1[0] * p1[0]) - (p1[1] * p1[1]) - (p1[2] * p1[2]);
	const double t2 = 0.5 - (p2[0] * p2[0]) - (p2[1] * p2[1]) - (p2[2] * p2[2]);
	const double t3 = 0.5 - (p3[0] * p3[0]) - (p3[1] * p3[1]) - (p3[2] * p3[2]);

	const dvec4 corner_contributions =
	{
		(t0 < 0) ? 0.0 : math::Pow4(t0) * math::DotGrad(p0_hash, p0[0], p0[1], p0[2]),
		(t1 < 0) ? 0.0 : math::Pow4(t1) * math::DotGrad(p1_hash, p1[0], p1[1], p1[2]),
		(t2 < 0) ? 0.0 : math::Pow4(t2) * math::DotGrad(p2_hash, p2[0], p2[1], p2[2]),
		(t3 < 0) ? 0.0 : math::Pow4(t3) * math::DotGrad(p3_hash, p3[0], p3[1], p3[2])
	};

	return std::accumulate(corner_contributions.begin(), corner_contributions.end(), 0.0); // normalize
}

double SimplexNoise::Sample(double x, double y, double z, double w) const noexcept
{
	return 0.0;
}

void SimplexNoise::GetP2P3Offsets(const dvec3& p1, ivec3& p2_offsets, ivec3& p3_offsets) const noexcept
{
	const double& x0 = p1[0];
	const double& y0 = p1[1];
	const double& z0 = p1[2];

	std::int64_t& i1 = p2_offsets[0];
	std::int64_t& j1 = p2_offsets[1];
	std::int64_t& k1 = p2_offsets[2];

	std::int64_t& i2 = p3_offsets[0];
	std::int64_t& j2 = p3_offsets[1];
	std::int64_t& k2 = p3_offsets[2];

	if (x0 >= y0)
	{
		if (y0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0;
			i2 = 1; j2 = 1; k2 = 0;
		} // X Y Z order
		else if (x0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0;
			i2 = 1; j2 = 0; k2 = 1;
		} // X Z Y order
		else
		{
			i1 = 0; j1 = 0; k1 = 1;
			i2 = 1; j2 = 0; k2 = 1;
		} // Z X Y order
	}
	else
	{
		if (y0 < z0)
		{
			i1 = 0; j1 = 0; k1 = 1;
			i2 = 0; j2 = 1; k2 = 1;
		} // Z Y X order
		else if (x0 < z0)
		{
			i1 = 0; j1 = 1; k1 = 0;
			i2 = 0; j2 = 1; k2 = 1;
		} // Y Z X order
		else
		{
			i1 = 0; j1 = 1; k1 = 0;
			i2 = 1; j2 = 1; k2 = 0;
		} // Y X Z order
	}
}