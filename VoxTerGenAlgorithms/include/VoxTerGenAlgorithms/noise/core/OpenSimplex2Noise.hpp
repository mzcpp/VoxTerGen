#ifndef OPEN_SIMPLEX_2_NOISE_HPP
#define OPEN_SIMPLEX_2_NOISE_HPP

#include <cstdint>

enum class OpenSimplex2Variant
{
	Fast,
	Smooth
};

class OpenSimplex2Noise
{
private:
	std::uint64_t seed_;

public:
	OpenSimplex2Noise(std::uint64_t seed);

	double Noise(double x) const;

	double Noise(double x, double y) const;

	double Noise(double x, double y, double z) const;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }

private:
	 void SkewCoordinates(double& x) const noexcept;

	 void SkewCoordinates(double& x, double& y) const noexcept;

	 void SkewCoordinates(double& x, double& y, double& z) const noexcept;
};

#endif // OPEN_SIMPLEX_2_NOISE_HPP

