#ifndef WORLEY_NOISE_HPP
#define WORLEY_NOISE_HPP

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"

#include <cstdint>
#include <array>

enum class DistanceResultType
{
	F1,
	F2,
	F3,
	F1_ADD_F2,
	F2_SUB_F1,
	F1_MUL_F2,
	F2_DIV_F1,
	F3_SUB_F1,
	CELL_VALUE
};

enum class DistanceMetric
{
	EUCLIDEAN,
	EUCLIDEAN_SQ,
	MANHATTAN,
	CHEBYSHEV,
	MINKOWSKI
};

enum class FeaturePointMode
{
	FIXED,
	WEIGHTED_RANDOM
};

using ivec3 = std::array<std::int64_t, 3>;
using dvec3 = std::array<double, 3>;

class WorleyNoise : public Noise
{
private:
	DistanceMetric dist_metric_;
	DistanceResultType dist_result_type_;
	FeaturePointMode fp_mode_;
	int n_feature_points_;
	float minkowski_p_;
	int dimension_;

public:
	WorleyNoise(
		std::uint64_t seed, 
		DistanceMetric dist_metric = DistanceMetric::EUCLIDEAN_SQ, 
		DistanceResultType dist_result_type = DistanceResultType::F1, 
		FeaturePointMode fp_mode = FeaturePointMode::FIXED,
		int n_feature_points = 1, 
		float minkowski_p = 0.0001f, 
		int dimension = 1);

	double Sample(double x) const noexcept override;

	double Sample(double x, double y) const noexcept override;

	double Sample(double x, double y, double z) const noexcept override;

	double Sample(double x, double y, double z, double w) const noexcept override;

	double NormalizeF1(double distance) const noexcept;

private:
	std::uint64_t HashCell(const ivec3& coords) const noexcept;

	std::uint64_t HashCellFast(const ivec3& coords) const noexcept;

	dvec3 GetRandomPoint(std::uint64_t hash, const ivec3& cell_coords) const noexcept;

	dvec3 GetRandomPointFast(std::uint64_t hash, const ivec3& cell_coords) const noexcept;

	double GetDistance(const dvec3& p1, const dvec3& p2) const noexcept;

	double GetResult(const dvec3& distances) const noexcept;

	int GetFeaturePointsNumber(std::uint64_t cell_hash) const noexcept;

	void CalculateMinDistances(const dvec3& current_cell, const ivec3& neighbor_cell, dvec3& min_distances, double& closest_hash) const noexcept;

	void UpdateMinDistances(double distance, dvec3& min_distances) const noexcept;
};

#endif // WORLEY_NOISE_HPP

