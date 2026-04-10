#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/WorleyNoise.hpp"
#include "VoxTerGenAlgorithms/utils/Hash.hpp"

#include <cstdint>
#include <limits>
#include <cmath>

namespace hash_constants
{
	constexpr std::uint64_t A = 0x9E3779B97F4A7C15ULL;	
	constexpr std::uint64_t B = 0xBF58476D1CE4E5B9ULL;
	constexpr std::uint64_t C = 0x94D049BB133111EBULL;
	constexpr std::uint64_t D = 0xD2B74407B1CE6E93ULL;
} // namespace hash_constants

WorleyNoise::WorleyNoise(std::uint64_t seed, int cell_size) : 
	seed_(seed), 
	cell_size_(cell_size), 
	dist_metric_(DistanceMetric::EUCLIDEAN), 
	dist_result_type_(DistanceResultType::F2_SUB_F1), 
	n_feature_points_(1), 
	minkowski_p_(2.0)
{
}

double WorleyNoise::Noise(double x) const
{
	const int xi = static_cast<int>(x);

	std::uint64_t h = seed_;
	h ^= cell_xi * hash_constants::A;
	h = hash::SplitMix64(h);
	
	double min_d = std::numeric_limits<double>::max();

	for (int xo = xi - 1; xo < xi + 1; ++xo)
	{
		std::uint64_t h = base_hash ^ (i * hash_constants::D);
		h = hash::SplitMix64(h);

		const Point p = GetRandomPoint(h, xi);
	}

	return 0.0;
}

double WorleyNoise::Noise(double x, double y) const
{
	const int xi = static_cast<int>(x);
	const int yi = static_cast<int>(y);

	std::uint64_t base_hash = seed_;
	base_hash ^= cell_xi * hash_constants::A;
	base_hash = hash::SplitMix64(base_hash);
	base_hash ^= cell_yi * hash_constants::B;
	base_hash = hash::SplitMix64(base_hash);
	
	double min_d = std::numeric_limits<double>::max();

	for (int yo = yi - 1; yo < yi + 1; ++yo)
	{
		for (int xo = xi - 1; xo < xi + 1; ++xo)
		{
			for (int i = 0; i < n_feature_points_; ++i)
			{
				std::uint64_t h = base_hash ^ (i * hash_constants::D);
				h = hash::SplitMix64(h);

				const Point p = GetRandomPoint(h, xi, yi);
			}
		}
	}

	return 0.0;
}

double WorleyNoise::Noise(double x, double y, double z) const
{
	const int xi = static_cast<int>(x);
	const int yi = static_cast<int>(y);
	const int zi = static_cast<int>(z);
	
	std::uint64_t h = seed_;
	h ^= cell_xi * hash_constants::A;
	h = hash::SplitMix64(h);
	h ^= cell_yi * hash_constants::B;
	h = hash::SplitMix64(h);
	h ^= cell_zi * hash_constants::C;
	h = hash::SplitMix64(h);
	
	double min_d = std::numeric_limits<double>::max();
	
	for (int yo = yi - 1; yo < yi + 1; ++yo)
	{
		for (int zo = zi - 1; zo < zi + 1; ++zo)
		{
			for (int xo = xi - 1; xo < xi + 1; ++xo)
			{
				std::uint64_t h = base_hash ^ (i * hash_constants::D);
				h = hash::SplitMix64(h);

				const Point p = GetRandomPoint(h, xi, yi, zi);
			}
		}
	}

	return 0.0;
}

double WorleyNoise::GetRandomPoint(int cell_xi) const noexcept
{	
	const std::uint64_t xh = hash::SplitMix64(h);
	const double fp_x = cell_xi + (xh / std::numeric_limits<std::uint64_t>::max());
	
	return fp_x;
}

Vec2D WorleyNoise::GetRandomPoint(int cell_xi, int cell_yi) const noexcept
{	
	const std::uint64_t xh = hash::SplitMix64(h);
	const std::uint64_t yh = hash::SplitMix64(xh);
	const double fp_x = cell_xi + (xh / std::numeric_limits<std::uint64_t>::max());
	const double fp_y = cell_yi + (yh / std::numeric_limits<std::uint64_t>::max());

	return { fp_x, fp_y };
}

Vec3D WorleyNoise::GetRandomPoint(int cell_xi, int cell_yi, int cell_zi) const noexcept
{
	
	const std::uint64_t xh = hash::SplitMix64(h);
	const std::uint64_t yh = hash::SplitMix64(xh);
	const std::uint64_t zh = hash::SplitMix64(yh);
	const double fp_x = cell_xi + (xh / std::numeric_limits<std::uint64_t>::max());
	const double fp_y = cell_yi + (yh / std::numeric_limits<std::uint64_t>::max());
	const double fp_z = cell_zi + (zh / std::numeric_limits<std::uint64_t>::max());

	return { fp_x, fp_y, fp_z };
}

double WorleyNoise::GetDistance(Point p1, Point p2) const noexcept
{
	if (dist_metric_ == DistanceMetric::EUCLIDEAN)
	{

	}
	else if (dist_metric_ == DistanceMetric::EUCLIDEAN_SQ)
	{
		
	}
	else if (dist_metric_ == DistanceMetric::MANHATTAN)
	{
		
	}
	else if (dist_metric_ == DistanceMetric::CHEBYSHEV)
	{
		
	}
	else if (dist_metric_ == DistanceMetric::MINKOWSKI)
	{

	}

	return 0.0;
}

double WorleyNoise::GetResult() const noexcept
{
	if (dist_result_type_ == DistanceResultType::F1)
	{

	}
	else if (dist_result_type_ == DistanceResultType::F2_SUB_F1)
	{

	}
	else if ()
	{

	}

	return 0.0;
}