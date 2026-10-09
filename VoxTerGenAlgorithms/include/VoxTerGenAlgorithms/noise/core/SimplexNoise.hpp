#ifndef SIMPLEX_NOISE_HPP
#define SIMPLEX_NOISE_HPP

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"
#include "VoxTerGenAlgorithms/utils/Common.hpp"

#include <cstdint>
#include <array>

class SimplexNoise : public Noise
{
private:
public:
	SimplexNoise(std::uint64_t seed);

	double Sample(double x) const noexcept override;

	double Sample(double x, double y) const noexcept override;

	double Sample(double x, double y, double z) const noexcept override;

	double Sample(double x, double y, double z, double w) const noexcept override;

private:
	void GetP2P3Offsets(const dvec3& p1, ivec3& p2_offsets, ivec3& p3_offsets) const noexcept;
};

#endif // SIMPLEX_NOISE_HPP