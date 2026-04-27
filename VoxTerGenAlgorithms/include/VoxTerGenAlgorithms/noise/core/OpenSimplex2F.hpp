#ifndef OPEN_SIMPLEX_2_F_HPP
#define OPEN_SIMPLEX_2_F_HPP

#include <cstdint>
#include <array>

enum class Noise2DModifier
{
	Skew, 
	Default = Skew,
	ImproveX, 
};

enum class Noise3DModifier
{
	ImproveXY, 
	ImproveXZ, 
	Default = ImproveXZ,
	Fallback
};

enum class Noise4DModifier
{
	ImproveXYZ, 
	Default = ImproveXYZ,
	ImproveXYZ_ImproveXY, 
	ImproveXYZ_ImproveXZ, 
	ImproveXY_ImproveZW, 
	Fallback
};

using ivec2 = std::array<std::int64_t, 2>;
using ivec3 = std::array<std::int64_t, 3>;
using uivec2 = std::array<std::uint64_t, 2>;
using uivec3 = std::array<std::uint64_t, 3>;
using dvec2 = std::array<double, 2>;
using dvec3 = std::array<double, 3>;

class OpenSimplex2F
{
private:
	std::uint64_t seed_;
	Noise2DModifier noise_2d_modifier_;
	Noise3DModifier noise_3d_modifier_;
	Noise4DModifier noise_4d_modifier_;

public:
	OpenSimplex2F(
		std::uint64_t seed, 
		Noise2DModifier noise_2d_modifier = Noise2DModifier::Default,
		Noise3DModifier noise_3d_modifier = Noise3DModifier::Default, 
		Noise4DModifier noise_4d_modifier = Noise4DModifier::Default);

	double Noise(double x, double y) const noexcept;

	double Noise(double x, double y, double z) const noexcept;

	double Noise(double x, double y, double z, double w) const noexcept;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }
	Noise2DModifier GetNoise2DModifier() const noexcept { return noise_2d_modifier_; }
	Noise3DModifier GetNoise3DModifier() const noexcept { return noise_3d_modifier_; }
	Noise4DModifier GetNoise4DModifier() const noexcept { return noise_4d_modifier_; }

	// Setters
	void SetSeed(std::uint64_t seed) noexcept { seed_ = seed; }
	void SetNoise2DModifier(Noise2DModifier noise_2d_modifier) noexcept { noise_2d_modifier_ = noise_2d_modifier; }
	void SetNoise3DModifier(Noise3DModifier noise_3d_modifier) noexcept { noise_3d_modifier_ = noise_3d_modifier; }
	void SetNoise4DModifier(Noise4DModifier noise_4d_modifier) noexcept { noise_4d_modifier_ = noise_4d_modifier; }

private:
	void RotateCoords(double& xr, double& yr) const noexcept;

	void RotateCoords(double& xr, double& yr, double& zr) const noexcept;
	
	void RotateCoords(double& xr, double& yr, double& zr, double& wr) const noexcept;
};

#endif // OPEN_SIMPLEX_2_F_HPP