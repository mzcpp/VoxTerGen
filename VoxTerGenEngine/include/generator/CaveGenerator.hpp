#ifndef CAVE_GENERATOR_HPP
#define CAVE_GENERATOR_HPP

#include "VoxTerGenAlgorithms/utils/Common.hpp"

#include <cstdint>

enum class CaveType
{
    TUNNEL_CARVER, 
    VECTOR_FIELD_WALKER
};

class CaveGenerator
{
private:
    CaveType cave_type_;
    std::uint64_t seed_;

public:
    CaveGenerator(CaveType cave_type, std::uint64_t seed);

    void GenerateCarvedCave();

    // Getters
    CaveType GetNoiseType() const noexcept { return cave_type_; }

    // Setters
    void SetCaveType(CaveType cave_type) noexcept { cave_type_ = cave_type; }
};

#endif // CAVE_GENERATOR_HPP

