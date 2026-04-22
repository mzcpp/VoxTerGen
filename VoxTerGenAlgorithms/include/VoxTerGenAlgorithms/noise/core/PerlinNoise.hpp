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
	std::uint64_t Hash(std::int64_t x) const noexcept;

	std::uint64_t Hash(std::int64_t x, std::int64_t y) const noexcept;

	std::uint64_t Hash(std::int64_t x, std::int64_t y, std::int64_t z) const noexcept;

	double Fade(double t) const noexcept;

	double Lerp(double a, double b, double t) const noexcept;

	double DotGrad(std::uint64_t hash, double x) const noexcept;

	double DotGrad(std::uint64_t hash, double x, double y) const noexcept;

	double DotGrad(std::uint64_t hash, double x, double y, double z) const noexcept;
};

#endif // PERLIN_NOISE_HPP

