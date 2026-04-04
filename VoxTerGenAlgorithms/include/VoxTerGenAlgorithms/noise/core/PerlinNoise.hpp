#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include <cstdint>

class PerlinNoise
{
private:
	std::uint64_t seed_;
public:
	PerlinNoise(std::uint64_t seed);

	std::uint64_t Hash1D(int x) const;

	std::uint64_t Hash2D(int x, int y) const;

	std::uint64_t Hash3D(int x, int y, int z) const;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }
};

#endif // PERLIN_NOISE_HPP

