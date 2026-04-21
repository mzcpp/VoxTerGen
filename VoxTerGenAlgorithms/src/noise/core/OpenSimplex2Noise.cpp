#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2Noise.hpp"

#include <cstdint>
#include <numbers>

OpenSimplex2Noise::OpenSimplex2Noise(std::uint64_t seed) : seed_(seed)
{
}

double OpenSimplex2Noise::Noise(double x) const noexcept
{
	return 0.0;
}

double OpenSimplex2Noise::Noise(double x, double y) const noexcept
{
	return 0.0;
}

double OpenSimplex2Noise::Noise(double x, double y, double z) const noexcept
{
	return 0.0;
}

void OpenSimplex2Noise::SkewCoordinates(double& x) const noexcept
{
	const double F2 = 0.5 * (std::numbers::sqrt3 - 1.0);
	const double t = x * F2;
	
	x += t;
}

void OpenSimplex2Noise::SkewCoordinates(double& x, double& y) const noexcept
{
	const double F2 = 0.5 * (std::numbers::sqrt3 - 1.0);
	const double t = (x + y) * F2;
	
	x += t;
	y += t;
}

void OpenSimplex2Noise::SkewCoordinates(double& x, double& y, double& z) const noexcept
{
	const double F2 = 0.5 * (std::numbers::sqrt3 - 1.0);
	const double t = (x + y + z) * F2;

	x += t;
	y += t;
	z += t;
}