#ifndef OPEN_SIMPLEX_2_NOISE_HPP
#define OPEN_SIMPLEX_2_NOISE_HPP

#include <cstdint>
#include <array>

enum class OpenSimplex2Variant
{
	Fast, 
	Smooth
};

enum class Noise2DModifier
{
	Default = ImproveXY, 
	ImproveXY, 
	ImproveX, 
	Fallback
};

enum class Noise3DModifier
{
	Default = ImproveXZ, 
	ImproveXY, 
	ImproveXZ, 
	Fallback
};

enum class Noise4DModifier
{
	Default = ImproveXYZ, 
	ImproveXYZ, 
	ImproveXY, 
	ImproveXZ, 
	ImproveYZ, 
	ImproveXYZW, 
	Fallback
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
	Noise2DModifier noise_2d_modifier_;
	Noise3DModifier noise_3d_modifier_;
	Noise4DModifier noise_4d_modifier_;

public:
	OpenSimplex2Noise(
		std::uint64_t seed, 
		OpenSimplex2Variant variant, 
		Noise2DModifier noise_2d_modifier = Noise2DModifier::Default, 
		Noise3DModifier noise_3d_modifier = Noise3DModifier::Default, 
		Noise4DModifier noise_4d_modifier = Noise4DModifier::Default);

	double Noise(double x, double y) const noexcept;

	double Noise(double x, double y, double z) const noexcept;

	double Noise(double x, double y, double z, double w) const noexcept;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }
	OpenSimplex2Variant Variant() const noexcept { return variant_; }
	Noise2DModifier Noise2DModifier() const noexcept { return noise_2d_modifier; }
	Noise3DModifier Noise3DModifier() const noexcept { return noise_3d_modifier; }
	Noise4DModifier Noise4DModifier() const noexcept { return noise_4d_modifier; }

	// Setters
	void SetSeed(std::uint64_t seed) noexcept { seed_ = seed; }
	void SetVariant(OpenSimplex2Variant variant) noexcept { variant_ = variant; }
	void SetNoise2DModifier(Noise2DModifier noise_2d_modifier) noexcept { noise_2d_modifier_ = noise_2d_modifier; }
	void SetNoise3DModifier(Noise3DModifier noise_3d_modifier) noexcept { noise_3d_modifier_ = noise_3d_modifier; }
	void SetNoise4DModifier(Noise4DModifier noise_4d_modifier) noexcept { noise_4d_modifier_ = noise_4d_modifier; }

private:
	void Improve(double& rx, double& ry) const noexcept;

	void Improve(double& rx, double& ry, double& rz) const noexcept;
	
	void Improve(double& rx, double& ry, double& rz, double& rw) const noexcept;
};

#endif // OPEN_SIMPLEX_2_NOISE_HPP

