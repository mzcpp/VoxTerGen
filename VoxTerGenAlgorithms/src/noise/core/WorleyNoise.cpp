#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/WorleyNoise.hpp"
#include "VoxTerGenAlgorithms/utils/Hash.hpp"

#include <cstdint>
#include <limits>
#include <cmath>
#include <array>
#include <cassert>
#include <algorithm>
#include <numbers>

/**
 * @note Original author of the algorithm is Steven Worley. This source code was adapted from The Book of Shaders' implementation.
 * Sources: https://thebookofshaders.com/12/
 */

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
	constexpr double inv_ui64_t_max = 1.0 / std::numeric_limits<uint64_t>::max();
	constexpr int fp_threshold_40 = 100;
	constexpr int fp_threshold_30 = 180;
	constexpr int fp_threshold_20 = 230;
}

WorleyNoise::WorleyNoise(std::uint64_t seed, DistanceMetric dist_metric, DistanceResultType dist_result_type,
	FeaturePointMode fp_mode, int n_feature_points, float minkowski_p, int dimension) :
	seed_(seed),
	dist_metric_(dist_metric),
	dist_result_type_(dist_result_type),
	fp_mode_(fp_mode),
	n_feature_points_(std::clamp(n_feature_points, 1, 5)),
	minkowski_p_(std::max(minkowski_p, 0.0001f)),
	dimension_(std::clamp(dimension, 1, 3))
{
}

double WorleyNoise::Sample(double x) const noexcept
{
	const dvec3 current_cell_coords = { x, 0.0, 0.0 };
	const std::int64_t xi = static_cast<std::int64_t>(std::floor(x));

	dvec3 min_distances = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
	double closest_hash = 0.0;

	for (std::int64_t xo = xi - 1; xo <= xi + 1; ++xo)
	{
		CalculateMinDistances(current_cell_coords, { xo, 0, 0 }, min_distances, closest_hash);
	}

	if (dist_result_type_ == DistanceResultType::CELL_VALUE)
	{
		return closest_hash;
	}

	return GetResult(min_distances);
}

double WorleyNoise::Sample(double x, double y) const noexcept
{
	const dvec3 current_cell_coords = { x, y, 0.0 };
	const std::int64_t xi = static_cast<std::int64_t>(std::floor(x));
	const std::int64_t yi = static_cast<std::int64_t>(std::floor(y));

	dvec3 min_distances = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
	double closest_hash = 0.0;

	for (std::int64_t yo = yi - 1; yo <= yi + 1; ++yo)
	{
		for (std::int64_t xo = xi - 1; xo <= xi + 1; ++xo)
		{
			CalculateMinDistances(current_cell_coords, { xo, yo, 0 }, min_distances, closest_hash);
		}
	}

	if (dist_result_type_ == DistanceResultType::CELL_VALUE)
	{
		return closest_hash;
	}

	return GetResult(min_distances);
}

double WorleyNoise::Sample(double x, double y, double z) const noexcept
{
	const dvec3 current_cell_coords = { x, y, z };
	const std::int64_t xi = static_cast<std::int64_t>(std::floor(x));
	const std::int64_t yi = static_cast<std::int64_t>(std::floor(y));
	const std::int64_t zi = static_cast<std::int64_t>(std::floor(z));

	dvec3 min_distances = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
	double closest_hash = 0.0;

	for (std::int64_t yo = yi - 1; yo <= yi + 1; ++yo)
	{
		for (std::int64_t zo = zi - 1; zo <= zi + 1; ++zo)
		{
			for (std::int64_t xo = xi - 1; xo <= xi + 1; ++xo)
			{
				CalculateMinDistances(current_cell_coords, { xo, yo, zo }, min_distances, closest_hash);
			}
		}
	}

	if (dist_result_type_ == DistanceResultType::CELL_VALUE)
	{
		return closest_hash;
	}

	return GetResult(min_distances);
}

//double WorleyNoise::Sample(double x, double y, double z, double w) const noexcept
//{
//	return 0.0;
//}

std::uint64_t WorleyNoise::HashCell(const ivec3& coords) const noexcept
{
	std::uint64_t cell_hash = seed_;

	for (int i = 0; i < dimension_; ++i)
	{
		// Flip the most significant bit.
		const std::uint64_t coord = static_cast<std::uint64_t>(coords[i]) ^ 0x8000000000000000ULL;
		cell_hash ^= hash::SplitMix64(coord * hash_constants::constants[i]);
	}

	return hash::SplitMix64(cell_hash);
}

std::uint64_t WorleyNoise::HashCellFast(const ivec3& coords) const noexcept
{
	std::uint64_t cell_hash = seed_;

	return cell_hash;
}

dvec3 WorleyNoise::GetRandomPoint(std::uint64_t hash, const ivec3& cell_coords) const noexcept
{
	dvec3 result = { 0.0, 0.0, 0.0 };
	std::uint64_t h = hash;

	for (int i = 0; i < dimension_; ++i)
	{
		h = hash::SplitMix64(h);

		// [0, 2^53) -> [0, 1)
		// Discard the lower 11 bits, then multiply with (1 / 2^53).
		const double u = (h >> 11) * (1.0 / (1ULL << 53));
		result[i] = cell_coords[i] + u;
	}

	return result;
}

dvec3 WorleyNoise::GetRandomPointFast(std::uint64_t hash, const ivec3& cell_coords) const noexcept
{
	dvec3 result = { 0.0, 0.0, 0.0 };

	return result;
}

double WorleyNoise::GetDistance(const dvec3& p1, const dvec3& p2) const noexcept
{
	double result = 0.0;

	if (dist_metric_ == DistanceMetric::EUCLIDEAN || dist_metric_ == DistanceMetric::EUCLIDEAN_SQ)
	{
		for (int i = 0; i < dimension_; ++i)
		{
			const double diff = p2[i] - p1[i];
			result += diff * diff;
		}

		return dist_metric_ == DistanceMetric::EUCLIDEAN ? std::sqrt(result) : result;
	}
	else if (dist_metric_ == DistanceMetric::MANHATTAN)
	{
		for (int i = 0; i < dimension_; ++i)
		{
			result += std::fabs(p2[i] - p1[i]);
		}

		return result;
	}
	else if (dist_metric_ == DistanceMetric::CHEBYSHEV)
	{
		for (int i = 0; i < dimension_; ++i)
		{
			result = std::fmax(result, std::fabs(p2[i] - p1[i]));
		}

		return result;
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

double WorleyNoise::NormalizeF1(double distance) const noexcept
{
	if (dist_metric_ == DistanceMetric::EUCLIDEAN)
	{
		return dimension_ == 2 ? (distance / std::numbers::sqrt2) : (distance / std::numbers::sqrt3);
	}
	else if (dist_metric_ == DistanceMetric::EUCLIDEAN_SQ || dist_metric_ == DistanceMetric::MANHATTAN)
	{
		return distance / static_cast<double>(dimension_);
	}
	else if (dist_metric_ == DistanceMetric::CHEBYSHEV)
	{
		return distance;
	}
	else if (dist_metric_ == DistanceMetric::MINKOWSKI)
	{
		return (distance / (std::pow(dimension_, 1.0 / minkowski_p_)));
	}

	assert(false);
	return 0.0;
}

double WorleyNoise::GetResult(const dvec3& distances) const noexcept
{
	const double f1 = distances[0];
	const double f2 = distances[1];
	const double f3 = distances[2];

	switch (dist_result_type_)
	{
	case DistanceResultType::F1:
		return f1;
	case DistanceResultType::F2:
		return f2;
	case DistanceResultType::F3:
		return f3;
	case DistanceResultType::F1_ADD_F2:
		return f1 + f2;
	case DistanceResultType::F2_SUB_F1:
		return f2 - f1;
	case DistanceResultType::F1_MUL_F2:
		return f1 * f2;
	case DistanceResultType::F2_DIV_F1:
	{
		constexpr double min_distance = 1e-6;
		return f2 / std::max(f1, min_distance);
	}
	case DistanceResultType::F3_SUB_F1:
		return f3 - f1;
	default:
		assert(false && "Invalid DistanceResultType!");
		return 0.0;
	}

	return 0.0;
}

int WorleyNoise::GetFeaturePointsNumber(std::uint64_t cell_hash) const noexcept
{
	if (fp_mode_ == FeaturePointMode::FIXED)
	{
		return n_feature_points_;
	}
	else if (fp_mode_ == FeaturePointMode::WEIGHTED_RANDOM)
	{
		const int rem = cell_hash & 0xFF;

		// ~40%
		if (rem < fp_threshold_40)
		{
			return 1;
		}
		// ~30%
		else if (rem < fp_threshold_30)
		{
			return 2;
		}
		// ~20%
		else if (rem < fp_threshold_20)
		{
			return 3;
		}
		// ~10%
		else
		{
			return 4;
		}
	}

	assert(false);
	return 1;
}

void WorleyNoise::CalculateMinDistances(const dvec3& current_cell, const ivec3& neighbor_cell, dvec3& min_distances, double& closest_hash) const noexcept
{
	const std::uint64_t neighbor_cell_hash = HashCell(neighbor_cell);
	const int feature_points_count = GetFeaturePointsNumber(neighbor_cell_hash);

	for (int i = 0; i < feature_points_count; ++i)
	{
		const std::uint64_t point_hash = hash::SplitMix64(neighbor_cell_hash ^ (i * hash_constants::D));
		const dvec3 feature_point = GetRandomPoint(point_hash, neighbor_cell);
		const double distance = GetDistance(feature_point, current_cell);

		if (distance < min_distances[0])
		{
			closest_hash = (neighbor_cell_hash * inv_ui64_t_max);
		}

		UpdateMinDistances(distance, min_distances);

	}
}

void WorleyNoise::UpdateMinDistances(double distance, dvec3& min_distances) const noexcept
{
	if (distance < min_distances[0])
	{
		min_distances[2] = min_distances[1];
		min_distances[1] = min_distances[0];
		min_distances[0] = distance;
	}
	else if (distance < min_distances[1])
	{
		min_distances[2] = min_distances[1];
		min_distances[1] = distance;
	}
	else if (distance < min_distances[2])
	{
		min_distances[2] = distance;
	}
}