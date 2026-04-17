#include "pch.h"

#include "VoxTerGenAlgorithms/noise/core/PerlinNoise.hpp"
#include "VoxTerGenAlgorithms/utils/Hash.hpp"

#include <cstdint>
#include <cmath>
#include <cassert>

PerlinNoise::PerlinNoise(std::uint64_t seed) : seed_(seed)
{
}

double PerlinNoise::Noise(double x) const noexcept
{
	const std::int64_t xi = static_cast<std::int64_t>(std::floor(x));
	const double xd = x - xi;

	const std::uint64_t a = Hash(xi);
	const std::uint64_t b = Hash(xi + 1);

	const double u = Fade(xd);

	return Lerp(DotGrad(a, xd), DotGrad(b, xd - 1.0), u);
}

double PerlinNoise::Noise(double x, double y) const noexcept
{
	const std::int64_t xi = static_cast<std::int64_t>(std::floor(x));
	const std::int64_t yi = static_cast<std::int64_t>(std::floor(y));

	const double xd = x - xi;
	const double yd = y - yi;

	const std::uint64_t aa = Hash(xi, yi);
	const std::uint64_t ab = Hash(xi, yi + 1);
	const std::uint64_t ba = Hash(xi + 1, yi);
	const std::uint64_t bb = Hash(xi + 1, yi + 1);

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

	const std::uint64_t aaa = Hash(xi, yi, zi);
	const std::uint64_t aab = Hash(xi, yi, zi + 1);
	const std::uint64_t aba = Hash(xi, yi + 1, zi);
	const std::uint64_t abb = Hash(xi, yi + 1, zi + 1);
	const std::uint64_t baa = Hash(xi + 1, yi, zi);
	const std::uint64_t bab = Hash(xi + 1, yi, zi + 1);
	const std::uint64_t bba = Hash(xi + 1, yi + 1, zi);
	const std::uint64_t bbb = Hash(xi + 1, yi + 1, zi + 1);

	const double u = Fade(xd);
	const double v = Fade(yd);
	const double w = Fade(zd);

	const double x11 = Lerp(DotGrad(aaa, xd, yd, zd), DotGrad(baa, xd - 1.0, yd, zd), u);
	const double x12 = Lerp(DotGrad(aab, xd, yd, zd - 1.0), DotGrad(bab, xd - 1.0, yd, zd - 1.0), u);
	const double x21 = Lerp(DotGrad(aba, xd, yd - 1.0, zd), DotGrad(bba, xd - 1.0, yd - 1.0, zd), u);
	const double x22 = Lerp(DotGrad(abb, xd, yd - 1.0, zd - 1.0), DotGrad(bbb, xd - 1.0, yd - 1.0, zd - 1.0), u);

	const double y1 = Lerp(x11, x12, v);
	const double y2 = Lerp(x21, x22, v);

	return Lerp(y1, y2, w);
}

std::uint64_t PerlinNoise::Hash(std::int64_t x) const noexcept
{
	std::uint64_t h = seed_;

	h = hash::SplitMix64(h ^ static_cast<uint64_t>(x));

	return h;
}

std::uint64_t PerlinNoise::Hash(std::int64_t x, std::int64_t y) const noexcept
{
	std::uint64_t h = seed_;

	h = hash::SplitMix64(h ^ static_cast<uint64_t>(x));
	h = hash::SplitMix64(h ^ static_cast<uint64_t>(y));

	return h;
}

std::uint64_t PerlinNoise::Hash(std::int64_t x, std::int64_t y, std::int64_t z) const noexcept
{
	std::uint64_t h = seed_;

	h = hash::SplitMix64(h ^ static_cast<uint64_t>(x));
	h = hash::SplitMix64(h ^ static_cast<uint64_t>(y));
	h = hash::SplitMix64(h ^ static_cast<uint64_t>(z));

	return h;
}

double PerlinNoise::Fade(double t) const noexcept
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::Lerp(double a, double b, double t) const noexcept
{
	return a + t * (b - a);
}

double PerlinNoise::DotGrad(std::uint64_t hash, double x) const noexcept
{
	return (hash & 1) ? x : -x;
}

double PerlinNoise::DotGrad(std::uint64_t hash, double x, double y) const noexcept
{
	switch (hash & 0x7)
	{
	case 0x0:
		return x + y;
	case 0x1:
		return x;
	case 0x2:
		return x - y;
	case 0x3:
		return -y;
	case 0x4:
		return -x - y;
	case 0x5:
		return -x;
	case 0x6:
		return -x + y;
	case 0x7:
		return y;
	default:
		assert(false);
		return 0.0;
	}

	assert(false);
	return 0.0;
}

double PerlinNoise::DotGrad(std::uint64_t hash, double x, double y, double z) const noexcept
{
	switch (hash & 0xF)
	{
	case 0x0:
		return  x + y;
	case 0x1:
		return -x + y;
	case 0x2:
		return  x - y;
	case 0x3:
		return -x - y;
	case 0x4:
		return  x + z;
	case 0x5:
		return -x + z;
	case 0x6:
		return  x - z;
	case 0x7:
		return -x - z;
	case 0x8:
		return  y + z;
	case 0x9:
		return -y + z;
	case 0xA:
		return  y - z;
	case 0xB:
		return -y - z;
	case 0xC:
		return  y + x;
	case 0xD:
		return -y + x;
	case 0xE:
		return  y - x;
	case 0xF:
		return -y - x;
	default:
		assert(false);
		return 0.0;
	}

	assert(false);
	return 0.0;
}