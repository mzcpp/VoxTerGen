#ifndef WORLEY_NOISE_HPP
#define WORLEY_NOISE_HPP

#include <cstdint>

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
	CELL_OUTPUT
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
	RANDOM
}

struct Vec2D
{
	double x = 0.0;
	double y = 0.0;
};

struct Vec3D
{
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
};

class WorleyNoise
{
private:
	std::uint64_t seed_;
	int cell_size_;
	DistanceMetric dist_metric_;
	DistanceResultType dist_result_type_;
	int n_feature_points_;
	float minkowski_p_;

public:
	WorleyNoise(std::uint64_t seed, int cell_size);

	double Noise(double x) const;

	double Noise(double x, double y) const;

	double Noise(double x, double y, double z) const;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }

	// Setters
	void SetDistanceMetric(DistanceMetric dist_metric) noexcept { dist_metric_ = dist_metric; }
	void SetDistanceResultType(DistanceResultType dist_result_type) noexcept { dist_result_type_ = dist_result_type; }
	void SetFeaturePointsNumber(int n_feature_points) noexcept { n_feature_points_ = n_feature_points; }
	void SetMinkowskiExponent(float minkowski_p) noexcept { minkowski_p_ = minkowski_p; }

private:
	double GetRandomPoint(int cell_xi) const noexcept;

	Vec2D GetRandomPoint(int cell_xi, int cell_yi) const noexcept;

	Vec3D GetRandomPoint(int cell_xi, int cell_yi, int cell_zi) const noexcept;

	double GetDistance(Point p1, Point p2) const noexcept;
};

#endif // WORLEY_NOISE_HPP

