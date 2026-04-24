#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include "VoxTerGenAlgorithms/utils/Hash.hpp"

#include <cstdint>

class PerlinNoise
{
private:
	std::uint64_t seed_;

public:
	PerlinNoise(std::uint64_t seed);

	double Noise(double x) const noexcept;

	double Noise(double x, double y) const noexcept;

	double Noise(double x, double y, double z) const noexcept;

	double Noise(double x, double y, double z, double w) const noexcept;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }

	// Setters
	void SetSeed(std::uint64_t seed) noexcept { seed_ = seed; }

private:
};

#endif // PERLIN_NOISE_HPP

