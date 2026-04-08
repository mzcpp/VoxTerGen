#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/WorleyNoise.hpp"

#include <cstdint>
#include <limits>

WorleyNoise::WorleyNoise(std::uint64_t seed, int cell_size) : seed_(seed), cell_size_(cell_size)
{

}

double WorleyNoise::Noise(double x) const
{
	const double dx = x / static_cast<double>(cell_size_);

	const int xi = static_cast<int>(dx);

	double min_d = std::numeric_limits<double>::max();

	for (int xo = -1; xo < 2; ++xo)
	{

	}

	return 0.0;
}

double WorleyNoise::Noise(double x, double y) const
{
	const double dx = x / static_cast<double>(cell_size_);
	const double dy = y / static_cast<double>(cell_size_);

	const int xi = static_cast<int>(dx);
	const int yi = static_cast<int>(dy);

	double min_d = std::numeric_limits<double>::max();

	for (int yo = -1; yo < 2; ++yo)
	{
		for (int xo = -1; xo < 2; ++xo)
		{
			// generate feature point for the cell
			// measure distance and update if necessary
		}
	}

	return 0.0;
}

double WorleyNoise::Noise(double x, double y, double z) const
{
	const double dx = x / static_cast<double>(cell_size_);
	const double dy = y / static_cast<double>(cell_size_);
	const double dz = z / static_cast<double>(cell_size_);

	const int xi = static_cast<int>(dx);
	const int yi = static_cast<int>(dy);
	const int zi = static_cast<int>(dz);

	double min_d = std::numeric_limits<double>::max();

	for (int yo = -1; yo < 2; ++yo)
	{
		for (int zo = -1; zo < 2; ++zo)
		{
			for (int xo = -1; xo < 2; ++xo)
			{

			}
		}
	}

	return 0.0;
}

Point WorleyNoise::GenRandomPoint() const noexcept
{
	return Point();
}
