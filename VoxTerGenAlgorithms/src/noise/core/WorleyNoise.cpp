#include "pch.h"

#include "noise/core/WorleyNoise.hpp"

#include <cstdint>

WorleyNoise::WorleyNoise(std::uint64_t seed) : seed_(seed)
{

}

double WorleyNoise::Noise(double x) const
{
	return 0.0;
}

double WorleyNoise::Noise(double x, double y) const
{
	return 0.0;
}

double WorleyNoise::Noise(double x, double y, double z) const
{
	return 0.0;
}