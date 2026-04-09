#ifndef WORLEY_NOISE_HPP
#define WORLEY_NOISE_HPP

#include <cstdint>

enum class DistanceResultType
{
	F1,
	F2_SUB_F1
}

enum class DistanceMetric
{
	EUCLIDEAN, 
	EUCLIDEAN_SQ, 
	MANHATTAN, 
	CHEBYSHEV
};

class WorleyNoise
{
private:
	std::uint64_t seed_;
	int cell_size_;
	DistanceMetric dist_metric_;
	DistanceResultType dist_result_type_;
	int n_feature_points_;

public:
	WorleyNoise(std::uint64_t seed, int cell_size, DistanceMetric dist_metric, DistanceResultType dist_result_type, int n_feature_points = 1);

	double Noise(double x) const;

	double Noise(double x, double y) const;

	double Noise(double x, double y, double z) const;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }

private:

};

#endif // WORLEY_NOISE_HPP

