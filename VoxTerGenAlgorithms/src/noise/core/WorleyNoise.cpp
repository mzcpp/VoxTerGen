#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/WorleyNoise.hpp"
#include "VoxTerGenAlgorithms/utils/Hash.hpp"

#include <cstdint>
#include <limits>
#include <cmath>
#include <array>
#include <cassert>

namespace hash_constants
{
	constexpr std::uint64_t A = 0x9E3779B97F4A7C15ULL;
	constexpr std::uint64_t B = 0xBF58476D1CE4E5B9ULL;
	constexpr std::uint64_t C = 0x94D049BB133111EBULL;
	constexpr std::uint64_t D = 0xD2B74407B1CE6E93ULL;
	constexpr std::array<std::uint64_t, 4> constants = { A, B, C, D };
} // namespace hash_constants

namespace
{
	constexpr double inv_u64_t_max = 1.0 / std::numeric_limits<uint64_t>::max();
}

WorleyNoise::WorleyNoise(std::uint64_t seed, int cell_size, DistanceMetric dist_metric, DistanceResultType dist_result_type, 
	FeaturePointMode fp_mode, int n_feature_points, float minkowski_p, int dimension) :
	seed_(seed), 
	cell_size_(cell_size), 
	dist_metric_(dist_metric),
	dist_result_type_(dist_result_type),
	fp_mode_(fp_mode),
	n_feature_points_(n_feature_points),
	minkowski_p_(minkowski_p),
	dimension_(std::max(1, dimension))
{
}

double WorleyNoise::Noise(double x) const
{
	const int xi = static_cast<int>(x);
	const ivec3 cell_coords = { static_cast<std::uint64_t>(xi), 0, 0 };
	const std::uint64_t base_hash = HashCell(cell_coords);

	if (dist_result_type_ == DistanceResultType::CELL_HASH_VALUE)
	{
		return static_cast<double>(base_hash) * inv_u64_t_max;
	}

	dvec3 min_distances = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };

	for (int xo = xi - 1; xo < xi + 1; ++xo)
	{
		for (int i = 0; i < GetFeaturePointsNumber(base_hash); ++i)
		{
			const std::uint64_t point_hash = hash::SplitMix64(base_hash ^ (i * hash_constants::D));
			const dvec3 random_point  = GetRandomPoint(point_hash, cell_coords);

			for (std::size_t j = 0; j < min_distances.size(); ++j)
			{
				const double dist = GetDistance(random_point, { x, 0.0, 0.0 });
				
				if (dist < min_distances[j])
				{
					min_distances[j] = dist;
					break;
				}
			}
		}
	}

	return GetResult(min_distances);
}

double WorleyNoise::Noise(double x, double y) const
{
	const int xi = static_cast<int>(x);
	const int yi = static_cast<int>(y);
	const ivec3 cell_coords = { static_cast<std::uint64_t>(xi), static_cast<std::uint64_t>(yi), 0 };
	const std::uint64_t base_hash = HashCell(cell_coords);

	if (dist_result_type_ == DistanceResultType::CELL_HASH_VALUE)
	{
		return static_cast<double>(base_hash) * inv_u64_t_max;
	}

	dvec3 min_distances = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };

	for (int yo = yi - 1; yo < yi + 1; ++yo)
	{
		for (int xo = xi - 1; xo < xi + 1; ++xo)
		{
			for (int i = 0; i < GetFeaturePointsNumber(base_hash); ++i)
			{
				const std::uint64_t point_hash = hash::SplitMix64(base_hash ^ (i * hash_constants::D));
				const dvec3 random_point = GetRandomPoint(point_hash, cell_coords);

				for (std::size_t j = 0; j < min_distances.size(); ++j)
				{
					const double dist = GetDistance(random_point, { x, y, 0.0 });

					if (dist < min_distances[j])
					{
						min_distances[j] = dist;
						break;
					}
				}
			}
		}
	}

	return GetResult(min_distances);
}

double WorleyNoise::Noise(double x, double y, double z) const
{
	const int xi = static_cast<int>(x);
	const int yi = static_cast<int>(y);
	const int zi = static_cast<int>(z);
	const ivec3 cell_coords = { static_cast<std::uint64_t>(xi), static_cast<std::uint64_t>(yi), static_cast<std::uint64_t>(zi) };
	const std::uint64_t base_hash = HashCell(cell_coords);

	if (dist_result_type_ == DistanceResultType::CELL_HASH_VALUE)
	{
		return static_cast<double>(base_hash) * inv_u64_t_max;
	}

	dvec3 min_distances = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };

	for (int yo = yi - 1; yo < yi + 1; ++yo)
	{
		for (int zo = zi - 1; zo < zi + 1; ++zo)
		{
			for (int xo = xi - 1; xo < xi + 1; ++xo)
			{
				for (int i = 0; i < GetFeaturePointsNumber(base_hash); ++i)
				{
					const std::uint64_t point_hash = hash::SplitMix64(base_hash ^ (i * hash_constants::D));
					const dvec3 random_point = GetRandomPoint(point_hash, cell_coords);

					for (std::size_t j = 0; j < min_distances.size(); ++j)
					{
						const double dist = GetDistance(random_point, { x, y, z });

						if (dist < min_distances[j])
						{
							min_distances[j] = dist;
							break;
						}
					}
				}
			}
		}
	}

	return GetResult(min_distances);
}

std::uint64_t WorleyNoise::HashCell(const ivec3& coords) const noexcept
{
	std::uint64_t base_hash = seed_;

	for (int i = 0; i < dimension_; ++i)
	{
		base_hash ^= coords[i] * hash_constants::constants[i];
		base_hash = hash::SplitMix64(base_hash);
	}

	return base_hash;
}

dvec3 WorleyNoise::GetRandomPoint(std::uint64_t hash, const ivec3& cell_coords) const noexcept
{
	dvec3 result = { 0.0, 0.0, 0.0 };
	ivec3 hashes = { hash::SplitMix64(hash), 0, 0 };

	for (int i = 1; i < dimension_; ++i)
	{
		hashes[i] = hash::SplitMix64(hashes[i - 1]);
	}

	for (int i = 0; i < dimension_; ++i)
	{
		result[i] = cell_coords[i] + (hashes[i] * inv_u64_t_max);
	}

	return result;
}

double WorleyNoise::GetDistance(dvec3 p1, dvec3 p2) const noexcept
{
	double result = 0.0;

	if (dist_metric_ == DistanceMetric::EUCLIDEAN || dist_metric_ == DistanceMetric::EUCLIDEAN_SQ)
	{
		for (int i = 0; i < dimension_; ++i)
		{
			result += (p2[i] - p1[i]) * (p2[i] - p1[i]);
		}

		return dist_metric_ == DistanceMetric::EUCLIDEAN ? std::sqrt(result) : result;
	}
	else if (dist_metric_ == DistanceMetric::MANHATTAN)
	{
		for (int i = 0; i < dimension_; ++i)
		{
			result = std::fmax(result, (p2[i] - p1[i]) * (p2[i] - p1[i]));
		}

		return result;
	}
	else if (dist_metric_ == DistanceMetric::CHEBYSHEV)
	{
		for (int i = 0; i < dimension_; ++i)
		{
			result += (p2[i] - p1[i]);
		}
	}
	else if (dist_metric_ == DistanceMetric::MINKOWSKI)
	{
		for (int i = 0; i < dimension_; ++i)
		{
			result += std::pow(std::fabs(p2[i] - p1[i]), minkowski_p_);
		}

		return std::pow(result, 1.0 / minkowski_p_);
	}

	assert(false);
	return 0.0;
}

double WorleyNoise::GetResult(const dvec3& distances) const noexcept
{
	if (dist_result_type_ == DistanceResultType::F1)
	{
		return distances[0];
	}
	else if (dist_result_type_ == DistanceResultType::F2)
	{
		return distances[1];
	}
	else if (dist_result_type_ == DistanceResultType::F3)
	{
		return distances[2];
	}
	else if (dist_result_type_ == DistanceResultType::F1_ADD_F2)
	{
		return distances[0] + distances[1];
	}
	else if (dist_result_type_ == DistanceResultType::F2_SUB_F1)
	{
		return distances[1] - distances[0];
	}
	else if (dist_result_type_ == DistanceResultType::F1_MUL_F2)
	{
		return distances[0] * distances[1];
	}
	else if (dist_result_type_ == DistanceResultType::F2_DIV_F1)
	{
		return distances[1] / distances[0];
	}
	else if (dist_result_type_ == DistanceResultType::F3_SUB_F1)
	{
		return distances[2] - distances[0];
	}

	return 0.0;
}

int WorleyNoise::GetFeaturePointsNumber(std::uint64_t cell_hash) const noexcept
{
	if (fp_mode_ == FeaturePointMode::FIXED)
	{
		return n_feature_points_;
	}
	else if (fp_mode_ == FeaturePointMode::POISSON_APPROX)
	{
		const int rem = cell_hash % 0xFF;

		if (rem < 100)
		{
			return 1;
		}
		else if (rem < 180)
		{
			return 2;
		}
		else if (rem < 180)
		{
			return 3;
		}
		else
		{
			return 4;
		}
	}

	assert(false);
	return 1;
}