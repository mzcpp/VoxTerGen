#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2Noise.hpp"

#include <cstdint>
#include <numbers>
#include <array>
#include <cmath>

/**
 * @note Parts of this source code were adapted from Stefan Gustavson's paper on Ken Perlin's Simplex Noise. 
 * 		 Source: https://www.researchgate.net/publication/216813608_Simplex_noise_demystified
 */

OpenSimplex2Noise::OpenSimplex2Noise(std::uint64_t seed) : seed_(seed)
{
}

double OpenSimplex2Noise::Noise(double x, double y) const noexcept
{
	double n0 = 0.0;
	double n1 = 0.0;
	double n2 = 0.0;

	// Skew coordinates to determine the simplex we're in.
	const double F2 = 0.5 * (std::numbers::sqrt3 - 1.0);
	const double s = (x + y) * F2;
	const ivec2 simplex_coords = { static_cast<int>(std::floor(x + s)), static_cast<int>(std::floor(y + s)) };

	// Unskew the cell origin back to axis-aligned space to determine distance from origin.
	const double G2 = (3.0 - std::numbers::sqrt3) / 6.0;
	const double t = (simplex_coords[0] + simplex_coords[1]) * G2;
	const dvec2 p1 = { x - (simplex_coords[0] - t), y - (simplex_coords[1] - t) };
	
	// Find the coordinates of the other 2 corners.
	const ivec2 p2_offsets = { p1[0] > p1[1], p1[0] <= p1[1] };
	const dvec2 p2 = { p1[0] - p2_offsets[0] + G2, p1[1] - p2_offsets[1] + G2 };
	const dvec2 p3 = { p1[0] - 1.0 + (2.0 * G2), p1[1] - 1.0 + (2.0 * G2) };


	
	return (n0 + n1 + n2); // scale by something to normalize?
}

double OpenSimplex2Noise::Noise(double x, double y, double z) const noexcept
{
	double n0 = 0.0;
	double n1 = 0.0;
	double n2 = 0.0;
	double n3 = 0.0;

	// Skew coordinates to determine the simplex we're in.
	const double F3 = 1.0 / 3.0;
	const double s = (x + y + z) * F3;
	const ivec3 simplex_coords = { static_cast<int>(std::floor(x + s)), static_cast<int>(std::floor(y + s)), static_cast<int>(std::floor(z + s)) };

	// Unskew the cell origin back to axis-aligned space to determine distance from origin.
	const double G3 = 1.0 / 6.0;
	const double t = (simplex_coords[0] + simplex_coords[1] + simplex_coords[2]) * G3;
	const dvec3 p1 = { x - (simplex_coords[0] - t), y - (simplex_coords[1] - t), z - (simplex_coords[2] - t) };

	ivec3 p2_offsets = { 0, 0, 0 };
	ivec3 p3_offsets = { 0, 0, 0 };

	GetP2P3Offsets(p1, p2_offsets, p3_offsets);

	return (n0 + n1 + n2 + n3); // scale by something to normalize?
}

void OpenSimplex2Noise::GetP2P3Offsets(const dvec3& p1, ivec3& p2_offsets, ivec3& p3_offsets) const noexcept
{
	const double& x0 = p1[0];
	const double& y0 = p1[1];
	const double& z0 = p1[2];

	int& i1 = p2_offsets[0];
	int& j1 = p2_offsets[1];
	int& k1 = p2_offsets[2];

	int& i2 = p3_offsets[0];
	int& j2 = p3_offsets[1];
	int& k2 = p3_offsets[2];

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