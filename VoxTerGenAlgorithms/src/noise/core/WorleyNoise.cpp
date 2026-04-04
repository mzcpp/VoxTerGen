#include "pch.h"

#include "noise/core/WorleyNoise.hpp"
#include "utils/Rng.hpp"

#include <cstdint>

WorleyNoise::WorleyNoise(std::uint64_t seed) : seed_(seed)
{

}
