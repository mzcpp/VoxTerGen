#ifndef OPEN_SIMPLEX2F_NOISE_HPP
#define OPEN_SIMPLEX2F_NOISE_HPP

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"

#include <cstdint>
#include <array>

enum class OpenSimplex2FNoise2DModifier
{
	Skew,
	Default = Skew,
	ImproveX,
};

enum class OpenSimplex2FNoise3DModifier
{
	ImproveXY,
	ImproveXZ,
	Default = ImproveXZ,
	Fallback
};

enum class OpenSimplex2FNoise4DModifier
{
	ImproveXYZ,
	Default = ImproveXYZ,
	ImproveXYZ_ImproveXY,
	ImproveXYZ_ImproveXZ,
	ImproveXY_ImproveZW,
	Fallback
};

class OpenSimplex2FNoise : public Noise
{
private:
	OpenSimplex2FNoise2DModifier noise_2d_modifier_;
	OpenSimplex2FNoise3DModifier noise_3d_modifier_;
	OpenSimplex2FNoise4DModifier noise_4d_modifier_;

public:
	OpenSimplex2FNoise(
		std::uint64_t seed,
		OpenSimplex2FNoise2DModifier noise_2d_modifier = OpenSimplex2FNoise2DModifier::Default,
		OpenSimplex2FNoise3DModifier noise_3d_modifier = OpenSimplex2FNoise3DModifier::Default,
		OpenSimplex2FNoise4DModifier noise_4d_modifier = OpenSimplex2FNoise4DModifier::Default);

	double Sample(double x) const noexcept override;

	double Sample(double x, double y) const noexcept override;

	double Sample(double x, double y, double z) const noexcept override;

	double Sample(double x, double y, double z, double w) const noexcept override;

	// Getters
	OpenSimplex2FNoise2DModifier GetNoise2DModifier() const noexcept { return noise_2d_modifier_; }
	OpenSimplex2FNoise3DModifier GetNoise3DModifier() const noexcept { return noise_3d_modifier_; }
	OpenSimplex2FNoise4DModifier GetNoise4DModifier() const noexcept { return noise_4d_modifier_; }

	// Setters
	void SetNoise2DModifier(OpenSimplex2FNoise2DModifier noise_2d_modifier) noexcept { noise_2d_modifier_ = noise_2d_modifier; }
	void SetNoise3DModifier(OpenSimplex2FNoise3DModifier noise_3d_modifier) noexcept { noise_3d_modifier_ = noise_3d_modifier; }
	void SetNoise4DModifier(OpenSimplex2FNoise4DModifier noise_4d_modifier) noexcept { noise_4d_modifier_ = noise_4d_modifier; }

private:
	void RotateCoords(double& xr, double& yr) const noexcept;

	void RotateCoords(double& xr, double& yr, double& zr) const noexcept;

	void RotateCoords(double& xr, double& yr, double& zr, double& wr) const noexcept;
};

#endif // OPEN_SIMPLEX2F_NOISE_HPP