#ifndef OPEN_SIMPLEX_2_NOISE_HPP
#define OPEN_SIMPLEX_2_NOISE_HPP

#include <cstdint>

enum class OpenSimplex2Variant
{
	Fast, 
	Smooth
}

class OpenSimplex2SNoise
{
private:
	std::uint64_t seed_;

public:
	OpenSimplex2SNoise(std::uint64_t seed);

	double Noise(double x) const;

	double Noise(double x, double y) const;

	double Noise(double x, double y, double z) const;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }

private:
	// void SkewCoordinates(double& x);

	// void SkewCoordinates(double& x, double& y);

	// void SkewCoordinates(double& x, double& y, double& z);
};

#endif // OPEN_SIMPLEX_2_NOISE_HPP

