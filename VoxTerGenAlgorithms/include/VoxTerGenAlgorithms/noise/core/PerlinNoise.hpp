#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"

#include <cstdint>

class PerlinNoise : public Noise
{
private:
public:
	PerlinNoise(std::uint64_t seed);

	double Sample(double x) const noexcept override;

	double Sample(double x, double y) const noexcept override;

	double Sample(double x, double y, double z) const noexcept override;

	double Sample(double x, double y, double z, double w) const noexcept override;
};

#endif // PERLIN_NOISE_HPP

