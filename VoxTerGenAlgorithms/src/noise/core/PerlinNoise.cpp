#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/PerlinNoise.hpp"
#include "VoxTerGenAlgorithms/utils/Hash.hpp"
#include "VoxTerGenAlgorithms/utils/Math.hpp"

#include <cstdint>
#include <cmath>
#include <cassert>

/**
 * @note Parts of this source code were adapted from Ken Perlin's original implementation available at: https://cs.nyu.edu/~perlin/noise/
 * 		 DotGrad functions were taken from: http://riven8192.blogspot.com/2010/08/calculate-perlinnoise-twice-as-fast.html
 */

PerlinNoise::PerlinNoise(std::uint64_t seed) : seed_(seed)
{
}

double PerlinNoise::Noise(double x) const noexcept
{
	const std::int64_t xi = static_cast<std::int64_t>(std::floor(x));
	const double xd = x - xi;

	const std::uint64_t a = hash::HashCoords(seed_, xi);
	const std::uint64_t b = hash::HashCoords(seed_, xi + 1);

	const double u = Fade(xd);

	return Lerp(DotGrad(a, xd), DotGrad(b, xd - 1.0), u);
}

double PerlinNoise::Noise(double x, double y) const noexcept
{
	const std::int64_t xi = static_cast<std::int64_t>(std::floor(x));
	const std::int64_t yi = static_cast<std::int64_t>(std::floor(y));

	const double xd = x - xi;
	const double yd = y - yi;

	const std::uint64_t aa = hash::HashCoords(seed_, xi, yi);
	const std::uint64_t ab = hash::HashCoords(seed_, xi, yi + 1);
	const std::uint64_t ba = hash::HashCoords(seed_, xi + 1, yi);
	const std::uint64_t bb = hash::HashCoords(seed_, xi + 1, yi + 1);

	const double u = Fade(xd);
	const double v = Fade(yd);

	const double x1 = Lerp(DotGrad(aa, xd, yd), DotGrad(ba, xd - 1.0, yd), u);
	const double x2 = Lerp(DotGrad(ab, xd, yd - 1.0), DotGrad(bb, xd - 1.0, yd - 1.0), u);

	return Lerp(x1, x2, v);
}

double PerlinNoise::Noise(double x, double y, double z) const noexcept
{
	const std::int64_t xi = static_cast<std::int64_t>(std::floor(x));
	const std::int64_t yi = static_cast<std::int64_t>(std::floor(y));
	const std::int64_t zi = static_cast<std::int64_t>(std::floor(z));

	const double xd = x - xi;
	const double yd = y - yi;
	const double zd = z - zi;

	const std::uint64_t aaa = hash::HashCoords(seed_, xi, yi, zi);
	const std::uint64_t aab = hash::HashCoords(seed_, xi, yi, zi + 1);
	const std::uint64_t aba = hash::HashCoords(seed_, xi, yi + 1, zi);
	const std::uint64_t abb = hash::HashCoords(seed_, xi, yi + 1, zi + 1);
	const std::uint64_t baa = hash::HashCoords(seed_, xi + 1, yi, zi);
	const std::uint64_t bab = hash::HashCoords(seed_, xi + 1, yi, zi + 1);
	const std::uint64_t bba = hash::HashCoords(seed_, xi + 1, yi + 1, zi);
	const std::uint64_t bbb = hash::HashCoords(seed_, xi + 1, yi + 1, zi + 1);

	const double u = Fade(xd);
	const double v = Fade(yd);
	const double w = Fade(zd);

	const double x11 = Lerp(math::DotGrad(aaa, xd, yd, zd), math::DotGrad(baa, xd - 1.0, yd, zd), u);
	const double x12 = Lerp(math::DotGrad(aab, xd, yd, zd - 1.0), math::DotGrad(bab, xd - 1.0, yd, zd - 1.0), u);
	const double x21 = Lerp(math::DotGrad(aba, xd, yd - 1.0, zd), math::DotGrad(bba, xd - 1.0, yd - 1.0, zd), u);
	const double x22 = Lerp(math::DotGrad(abb, xd, yd - 1.0, zd - 1.0), math::DotGrad(bbb, xd - 1.0, yd - 1.0, zd - 1.0), u);

	const double y1 = Lerp(x11, x12, v);
	const double y2 = Lerp(x21, x22, v);

	return Lerp(y1, y2, w);
}

double PerlinNoise::Noise(double x, double y, double z, double w) const noexcept
{
	// TODO
	return 0.0;
}

double PerlinNoise::Fade(double t) const noexcept
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::Lerp(double a, double b, double t) const noexcept
{
	return a + t * (b - a);
}