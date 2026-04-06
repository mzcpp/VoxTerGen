#include "pch.h"

#include "noise/core/OpenSimplex2SNoise.hpp"

#include <cstdint>

OpenSimplex2SNoise::OpenSimplex2SNoise(std::uint64_t seed) : seed_(seed)
{

}

double OpenSimplex2SNoise::Noise(double x) const
{
	return 0.0;
}

double OpenSimplex2SNoise::Noise(double x, double y) const
{
	return 0.0;
}

double OpenSimplex2SNoise::Noise(double x, double y, double z) const
{
	return 0.0;
}