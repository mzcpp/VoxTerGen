#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/WorleyNoise.hpp"
#include "VoxTerGenAlgorithms/utils/Hash.hpp"

#include <cstdint>
#include <limits>

namespace hash_constants
{
	constexpr std::uint64_t A = 0x9E3779B97F4A7C15;	
	constexpr std::uint64_t B = 0xBF58476D1CE4E5B9;
	constexpr std::uint64_t C = 0x94D049BB133111EB;
} // namespace hash_constants

WorleyNoise::WorleyNoise(std::uint64_t seed, int cell_size, DistanceMetric dist_metric, 
	DistanceResultType dist_result_type, int n_feature_points) : 
	seed_(seed), 
	cell_size_(cell_size), 
	dist_metric_(dist_metric), 
	dist_result_type_(dist_result_type), 
	n_feature_points_(n_feature_points)
{
}

double WorleyNoise::Noise(double x) const
{
	const double dx = x / static_cast<double>(cell_size_);

	const int xi = static_cast<int>(dx);

	double min_d = std::numeric_limits<double>::max();

	std::uint64_t h = seed_;

	h ^= x * hash_constants::A;
	h = hash::SplitMix64(h);

	std::uint64_t px = hash::SplitMix64(h);

	for (int xo = xi - 1; xo < xi + 1; ++xo)
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

	std::uint64_t h = seed_;

	h ^= x * hash_constants::A;
	h = hash::SplitMix64(h);
	h ^= y * hash_constants::B;
	h = hash::SplitMix64(h);

	std::uint64_t px = hash::SplitMix64(h);
	std::uint64_t py = hash::SplitMix64(px);

	for (int yo = yi - 1; yo < yi + 1; ++yo)
	{
		for (int xo = xi - 1; xo < xi + 1; ++xo)
		{
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

	std::uint64_t h = seed_;

	h ^= x * hash_constants::A;
	h = hash::SplitMix64(h);
	h ^= y * hash_constants::B;
	h = hash::SplitMix64(h);
	h ^= z * hash_constants::C;
	h = hash::SplitMix64(h);

	std::uint64_t fp_x = hash::SplitMix64(h);
	std::uint64_t fp_y = hash::SplitMix64(px);
	std::uint64_t fp_z = hash::SplitMix64(py);

	// normalize [0, 1)

	for (int yo = yi - 1; yo < yi + 1; ++yo)
	{
		for (int zo = zi - 1; zo < zi + 1; ++zo)
		{
			for (int xo = xi - 1; xo < xi + 1; ++xo)
			{

			}
		}
	}

	return 0.0;
}
