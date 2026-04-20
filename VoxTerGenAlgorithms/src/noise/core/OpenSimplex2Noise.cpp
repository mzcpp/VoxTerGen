#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2Noise.hpp"

#include <cstdint>

OpenSimplex2Noise::OpenSimplex2Noise(std::uint64_t seed) : seed_(seed)
{

}

double OpenSimplex2Noise::Noise(double x) const
{
	return 0.0;
}

double OpenSimplex2Noise::Noise(double x, double y) const
{
	return 0.0;
}

double OpenSimplex2Noise::Noise(double x, double y, double z) const
{
	return 0.0;
}