#ifndef CAVE_GENERATOR_HPP
#define CAVE_GENERATOR_HPP

#include "VoxTerGenAlgorithms/utils/Common.hpp"

#include <cstdint>

enum class CaveType
{
    VECTOR_FIELD_WALKER
};

class CaveGenerator
{
private:
    CaveType cave_type_;
    std::uint64_t seed_;

public:
    CaveGenerator(CaveType cave_type, std::uint64_t seed);
};

#endif // CAVE_GENERATOR_HPP

