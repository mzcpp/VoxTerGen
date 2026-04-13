#ifndef WORLEY_NOISE_HPP
#define WORLEY_NOISE_HPP

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
	CELL_HASH_VALUE
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
	POISSON_APPROX
};

using ivec3 = std::array<std::uint64_t, 3>;
using dvec3 = std::array<double, 3>;

class WorleyNoise
{
private:
	std::uint64_t seed_;
	int cell_size_;
	DistanceMetric dist_metric_;
	DistanceResultType dist_result_type_;
	FeaturePointMode fp_mode_;
	int n_feature_points_;
	float minkowski_p_;
	int dimension_;
	double min_distance_;

public:
	WorleyNoise(std::uint64_t seed, int cell_size, DistanceMetric dist_metric, DistanceResultType dist_result_type, FeaturePointMode fp_mode,
		int n_feature_points, float minkowski_p, int dimension);

	double Noise(double x) const;

	double Noise(double x, double y) const;

	double Noise(double x, double y, double z) const;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }

private:
	std::uint64_t HashCell(const ivec3& coords) const noexcept;

	dvec3 GetRandomPoint(std::uint64_t hash, const ivec3& cell_coords) const noexcept;

	double GetDistance(dvec3 p1, dvec3 p2) const noexcept;

	double GetResult(const dvec3& distances) const noexcept;

	int GetFeaturePointsNumber(std::uint64_t cell_hash) const noexcept;

	dvec3 CalculateMinDistances(dvec3 current_cell, dvec3 neighbor_cell) const noexcept;

	void UpdateMinDistances(double distance, dvec3& min_distances) const noexcept;
};

#endif // WORLEY_NOISE_HPP

