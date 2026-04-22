#ifndef OPEN_SIMPLEX_2_NOISE_HPP
#define OPEN_SIMPLEX_2_NOISE_HPP

#include <cstdint>
#include <array>

enum class OpenSimplex2Variant
{
	Fast,
	Smooth
};

using ivec2 = std::array<std::int64_t, 2>;
using ivec3 = std::array<std::int64_t, 3>;
using uivec2 = std::array<std::uint64_t, 2>;
using uivec3 = std::array<std::uint64_t, 3>;
using dvec2 = std::array<double, 2>;
using dvec3 = std::array<double, 3>;

class OpenSimplex2Noise
{
private:
	std::uint64_t seed_;
	OpenSimplex2Variant variant_;

public:
	OpenSimplex2Noise(std::uint64_t seed);

	double Noise(double x, double y) const noexcept;

	double Noise(double x, double y, double z) const noexcept;

	double Noise(double x, double y, double z, double w) const noexcept;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }
	OpenSimplex2Variant Variant() const noexcept { return variant_; }

	// Setters
	void SetSeed(std::uint64_t seed) noexcept { seed_ = seed; }
	void SetVariant(OpenSimplex2Variant variant) noexcept { variant_ = variant; }

private:
};

#endif // OPEN_SIMPLEX_2_NOISE_HPP

