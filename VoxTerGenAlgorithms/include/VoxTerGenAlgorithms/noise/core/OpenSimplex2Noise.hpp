#ifndef OPEN_SIMPLEX_2_NOISE_HPP
#define OPEN_SIMPLEX_2_NOISE_HPP

#include <cstdint>
#include <array>

enum class OpenSimplex2Variant
{
	Fast,
	Smooth
};

using ivec2 = std::array<int, 2>;
using ivec3 = std::array<int, 3>;
using dvec2 = std::array<double, 2>;
using dvec3 = std::array<double, 3>;

class OpenSimplex2Noise
{
private:
	std::uint64_t seed_;

public:
	OpenSimplex2Noise(std::uint64_t seed);

	double Noise(double x, double y) const noexcept;

	double Noise(double x, double y, double z) const noexcept;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }

private:
	void GetP2P3Offsets(const dvec3& p1, ivec3& p2_offsets, ivec3& p3_offsets) const noexcept;
};

#endif // OPEN_SIMPLEX_2_NOISE_HPP

