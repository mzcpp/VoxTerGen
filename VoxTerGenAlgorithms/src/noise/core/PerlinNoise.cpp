#include "pch.h"

#include "noise/core/PerlinNoise.hpp"

#include <cstdint>

PerlinNoise::PerlinNoise(std::uint64_t seed) : seed_(seed)
{

}

std::uint64_t PerlinNoise::Hash1D(int x) const
{
	return 0;
}

std::uint64_t PerlinNoise::Hash2D(int x, int y) const
{
	return 0;
}

std::uint64_t PerlinNoise::Hash3D(int x, int y, int z) const 
{
	return 0;
}
