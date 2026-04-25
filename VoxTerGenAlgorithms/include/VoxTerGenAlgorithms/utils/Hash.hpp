#ifndef HASH_HPP
#define HASH_HPP

#include <cstdint>
#include <concepts>

namespace hash
{
    /**
     * @brief SplitMix64 pseudo-random number generator
     * @note Source: http://xorshift.di.unimi.it/splitmix64.c
     */
    inline std::uint64_t SplitMix64(std::uint64_t x) 
    {
        std::uint64_t z = x + 0x9E3779B97F4A7C15ULL;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }

    template<std::integral... Args>
    inline std::uint64_t HashCoords(std::uint64_t seed, Args... xyzw) noexcept
    {
        std::uint64_t h = seed;

        ((h = hash::SplitMix64(h ^ static_cast<std::uint64_t>(xyzw))), ...);

        return h;
    }
} // namespace hash

#endif // HASH_HPP
