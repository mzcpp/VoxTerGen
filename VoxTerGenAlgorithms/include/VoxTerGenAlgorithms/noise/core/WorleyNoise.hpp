#ifndef WORLEY_NOISE_HPP
#define WORLEY_NOISE_HPP

#include <cstdint>

class WorleyNoise
{
private:
	std::uint64_t seed_;

public:
	WorleyNoise(std::uint64_t seed);
};

#endif // WORLEY_NOISE_HPP

