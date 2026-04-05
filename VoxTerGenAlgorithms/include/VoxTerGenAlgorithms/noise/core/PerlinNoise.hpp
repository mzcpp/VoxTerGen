#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include <cstdint>
#include <cmath>

#include "utils/Hash.hpp"

/**
 * @note Portion was based on Ken Perlin's original implementation available at: https://cs.nyu.edu/~perlin/noise/
 */
class PerlinNoise
{
private:
	std::uint64_t seed_;

public:
	PerlinNoise(std::uint64_t seed) : seed_(seed)
	{
	}

	double Noise(double x) const
	{
		const int xi = static_cast<int>(std::floor(x));
		const double xd = x - xi;

		const int a = Hash(xi);
		const int b = Hash(xi + 1);

		const double u = Fade(xd);

		return Lerp(DotGrad(a, xd), DotGrad(b, xd - 1.0), u);
	}

	double Noise(double x, double y) const
	{
		const int xi = static_cast<int>(std::floor(x));
		const int yi = static_cast<int>(std::floor(y));

		const double xd = x - xi;
		const double yd = y - yi;

		const int aa = Hash(xi, yi);
		const int ab = Hash(xi, yi + 1);
		const int ba = Hash(xi + 1, yi);
		const int bb = Hash(xi + 1, yi + 1);

		const double u = Fade(xd);
		const double v = Fade(yd);

		const double x1 = Lerp(DotGrad(aa, xd, yd), DotGrad(ba, xd - 1.0, yd), u);
		const double x2 = Lerp(DotGrad(ab, xd, yd - 1.0), DotGrad(bb, xd - 1.0, yd - 1.0), u);

		return Lerp(x1, x2, v);
	}

	double Noise(double x, double y, double z) const
	{
		const int xi = static_cast<int>(std::floor(x));
		const int yi = static_cast<int>(std::floor(y));
		const int zi = static_cast<int>(std::floor(z));

		const double xd = x - xi;
		const double yd = y - yi;
		const double zd = z - zi;

		const int aaa = Hash(xi, yi, zi);
		const int aab = Hash(xi, yi, zi + 1);
		const int aba = Hash(xi, yi + 1, zi);
		const int abb = Hash(xi, yi + 1, zi + 1);
		const int baa = Hash(xi + 1, yi, zi);
		const int bab = Hash(xi + 1, yi, zi + 1);
		const int bba = Hash(xi + 1, yi + 1, zi);
		const int bbb = Hash(xi + 1, yi + 1, zi + 1);

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

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }

private:
	std::uint64_t Hash(std::int64_t x) const
	{
		std::uint64_t h = seed_;

		h = hash::SplitMix64(h ^ x, seed_);

		return h;
	}

	std::uint64_t Hash(std::int64_t x, std::int64_t y) const
	{
		std::uint64_t h = seed_;

		h = hash::SplitMix64(h ^ x, seed_);
		h = hash::SplitMix64(h ^ y, seed_);

		return h;
	}

	std::uint64_t Hash(std::int64_t x, std::int64_t y, std::int64_t z) const
	{
		std::uint64_t h = seed_;

		h = hash::SplitMix64(h ^ x, seed_);
		h = hash::SplitMix64(h ^ y, seed_);
		h = hash::SplitMix64(h ^ z, seed_);

		return h;
	}

	double Fade(double t) const
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	double Lerp(double t, double a, double b) const
	{
		return a + t * (b - a);
	}

	double DotGrad(std::int64_t hash, double x) const
	{
		return (hash & 1) ? x : -x;
	}

	double DotGrad(std::int64_t hash, double x, double y) const
	{
		const double u = (hash & 1) ? x : -x;
		const double v = (hash & 2) ? y : -y;

		return (hash & 4) ? (u + v) : (u - v);
	}

	double DotGrad(std::int64_t hash, double x, double y, double z) const
	{
		const int h = hash & 15;

		double u = h < 8 ? x : y;
		double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);

		u = (h & 1) ? -u : u;
		v = (h & 2) ? -v : v;

		return u + v;
	}
};

#endif // PERLIN_NOISE_HPP

