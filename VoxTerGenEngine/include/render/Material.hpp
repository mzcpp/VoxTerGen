#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <cstdint>

enum class Material : std::uint8_t 
{   
    Air = 0, 
    GrassTop, 
    GrassSide, 
    Dirt, 
    Stone, 
    Water, 
    Snow, 
    Sand, 
    Bedrock
};

#endif // MATERIAL_HPP