#ifndef TERRAIN_GENERATOR_HPP
#define TERRAIN_GENERATOR_HPP

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"
#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2FNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2SNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/PerlinNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/SimplexNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/WorleyNoise.hpp"

#include <memory>
#include <unordered_map>

enum class NoiseType
{
    PERLIN, 
    SIMPLEX, 
    WORLEY, 
    OPEN_SIMPLEX_2F, 
    OPEN_SIMPLEX_2S
};

class Noise

class TerrainGenerator
{
private:
    NoiseType noise_type_;
    std::uint64_t seed_;
    
    std::unordered_map<NoiseType, Noise> noises_;

public:
    TerrainGenerator(std::uint64_t seed);

    double GetCoordHeight(double x, double y);

    // Getters
    NoiseType GetNoiseType() const noexcept { return noise_type_; }

    // Setters
    void SetNoiseType(NoiseType noise_type) noexcept { noise_type_ = noise_type; }
};

#endif // TERRAIN_GENERATOR_HPP

