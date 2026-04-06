#ifndef WORLEY_NOISE_HPP
#define WORLEY_NOISE_HPP

#include <cstdint>

class WorleyNoise
{
private:
	std::uint64_t seed_;

public:
	WorleyNoise(std::uint64_t seed);

	double Noise(double x) const;

	double Noise(double x, double y) const;

	double Noise(double x, double y, double z) const;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }

private:

};

#endif // WORLEY_NOISE_HPP

