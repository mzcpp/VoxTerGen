#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include <cstdint>

#include "utils/Hash.hpp"

/**
 * @note Portion was based on Ken Perlin's original implementation available at: https://cs.nyu.edu/~perlin/noise/
 *		 DotGrad functions were taken from: http://riven8192.blogspot.com/2010/08/calculate-perlinnoise-twice-as-fast.html
 */
class PerlinNoise
{
private:
	std::uint64_t seed_;

public:
	PerlinNoise(std::uint64_t seed);

	double Noise(double x) const;

	double Noise(double x, double y) const;

	double Noise(double x, double y, double z) const;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }

private:
	std::uint64_t Hash(std::int64_t x) const;

	std::uint64_t Hash(std::int64_t x, std::int64_t y) const;

	std::uint64_t Hash(std::int64_t x, std::int64_t y, std::int64_t z) const;

	double Fade(double t) const;

	double Lerp(double a, double b, double n) const;

	double DotGrad(std::uint64_t hash, double x) const;

	double DotGrad(std::uint64_t hash, double x, double y) const;

	double DotGrad(std::uint64_t hash, double x, double y, double z) const;
};

#endif // PERLIN_NOISE_HPP

