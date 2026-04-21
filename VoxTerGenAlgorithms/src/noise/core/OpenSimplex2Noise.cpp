#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2Noise.hpp"

#include <cstdint>
#include <numbers>
#include <array>
#include <cmath>

/**
 * @note This source code was adapted from Kurt Spencer's and Jordan Peck's repositories.
 * Sources: https://github.com/KdotJPG/OpenSimplex2
 *			https://github.com/Auburn/FastNoiseLite
 */

OpenSimplex2Noise::OpenSimplex2Noise(std::uint64_t seed) : seed_(seed)
{
}

double OpenSimplex2Noise::Noise(double x, double y) const noexcept
{
	return 0.0;
}

double OpenSimplex2Noise::Noise(double x, double y, double z) const noexcept
{
	return 0.0;
}