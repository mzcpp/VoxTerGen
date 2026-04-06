#ifndef OPEN_SIMPLEX_2S_NOISE_HPP
#define OPEN_SIMPLEX_2S_NOISE_HPP

#include <cstdint>

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
};

#endif // OPEN_SIMPLEX_2S_NOISE_HPP

