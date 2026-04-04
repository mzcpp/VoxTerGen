#ifndef OPEN_SIMPLEX_2S_NOISE_HPP
#define OPEN_SIMPLEX_2S_NOISE_HPP

#include <cstdint>

class OpenSimplex2SNoise
{
private:
	std::uint64_t seed_;

public:
	OpenSimplex2SNoise(std::uint64_t seed);
};

#endif // OPEN_SIMPLEX_2S_NOISE_HPP

