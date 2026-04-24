#ifndef MATH_HPP
#define MATH_HPP

#include <cstdint>
#include <cassert>

namespace math
{
	inline double Lerp(double a, double b, double t) noexcept
	{
		return a + t * (b - a);
	}

	inline double Fade(double t) noexcept
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	inline double DotGrad(std::uint64_t hash, double x) noexcept
	{
		return (hash & 1) ? x : -x;
	}

	inline double DotGrad(std::uint64_t hash, double x, double y) noexcept
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

	inline double DotGrad(std::uint64_t hash, double x, double y, double z) noexcept
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
}

#endif
