#ifndef RNG_HPP
#define RNG_HPP

#include <cstdint>

#include "Hash.hpp"

namespace rng
{
	/**
	 * @brief Xoshiro256** pseudo-random number generator
	 * @note Source: https://xorshift.di.unimi.it/xoshiro256starstar.c
	 */
	class Xoshiro256StarStar
	{
	private:
		std::uint64_t s[4];
	
		std::uint64_t Rotl(const std::uint64_t x, int k) 
		{
			return (x << k) | (x >> (64 - k));
		}

	public:
		explicit Xoshiro256StarStar(std::uint64_t seed)
		{
			std::uint64_t x = seed;

			for (int i = 0; i < 4; ++i)
			{
				s[i] = hash::SplitMix64(x, seed);
				x = s[i];
			}
		}

		std::uint64_t Next() 
		{
			const std::uint64_t result = Rotl(s[1] * 5, 7) * 9;
			const std::uint64_t t = s[1] << 17;

			s[2] ^= s[0];
			s[3] ^= s[1];
			s[1] ^= s[2];
			s[0] ^= s[3];

			s[2] ^= t;

			s[3] = Rotl(s[3], 45);

			return result;
		}
	};
} // namespace rng

#endif // RNG_HPP
