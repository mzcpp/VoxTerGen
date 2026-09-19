#ifndef TERRAIN_GENERATOR_HPP
#define TERRAIN_GENERATOR_HPP

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"

#include "VoxTerGenAlgorithms/noise/methods/FractionalBrownianMotion.hpp"

#include "VoxTerGenAlgorithms/utils/Common.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

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

    FractionalBrownianMotion fbm_;
    
    std::unordered_map<NoiseType, std::unique_ptr<Noise>> noises_;

public:
    TerrainGenerator(NoiseType noise_type, std::uint64_t seed);

    void InitializeNoises();

    void GenerateChunkTerrainFromHeightMap(const std::shared_ptr<Chunk>& chunk, std::stop_token stop_token);

    // Getters
    NoiseType GetNoiseType() const noexcept { return noise_type_; }
    
    Noise* GetCurrentNoise() const noexcept
    {
        const auto noises_find_it = noises_.find(GetNoiseType());
        assert(noises_find_it != noises_.end());

        return noises_find_it->second.get();
    }

    // Setters
    void SetNoiseType(NoiseType noise_type) noexcept { noise_type_ = noise_type; }

private:
    double PowerCurve(double n, double exponent);

    double LogisticSCurve(double n, double k);

    double GetWarpVector(const Noise* noise, double x);

    glm::dvec2 GetWarpVector(const Noise* noise, double x, double y);

    glm::dvec3 GetWarpVector(const Noise* noise, double x, double y, double z);
};

#endif // TERRAIN_GENERATOR_HPP

