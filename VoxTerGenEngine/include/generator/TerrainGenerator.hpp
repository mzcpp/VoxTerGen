#ifndef TERRAIN_GENERATOR_HPP
#define TERRAIN_GENERATOR_HPP

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"

#include <memory>
#include <stop_token>
#include <unordered_map>

enum class NoiseType
{
    PERLIN, 
    SIMPLEX, 
    WORLEY, 
    OPEN_SIMPLEX_2F, 
    OPEN_SIMPLEX_2S
};

class Noise;
class Chunk;

class TerrainGenerator
{
private:
    NoiseType noise_type_;
    std::uint64_t seed_;
    
    std::unordered_map<NoiseType, std::unique_ptr<Noise>> noises_;

public:
    TerrainGenerator(NoiseType noise_type, std::uint64_t seed);

    void InitializeNoises();

    void GenerateChunkTerrain(const std::shared_ptr<Chunk>& chunk, std::stop_token stop_token);

    double GetCoordHeight(double x, double y);

    // Getters
    NoiseType GetNoiseType() const noexcept { return noise_type_; }

    // Setters
    void SetNoiseType(NoiseType noise_type) noexcept { noise_type_ = noise_type; }
};

#endif // TERRAIN_GENERATOR_HPP

