#include "generator/TerrainGenerator.hpp"

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"
#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2FNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2SNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/PerlinNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/SimplexNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/WorleyNoise.hpp"

#include "VoxTerGenAlgorithms/utils/Common.hpp"

#include "world/Chunk.hpp"

#include <memory>
#include <stop_token>
#include <unordered_map>
#include <cmath>
#include <algorithm>

TerrainGenerator::TerrainGenerator(NoiseType noise_type, std::uint64_t seed) : seed_(seed), noise_type_(noise_type)
{
	InitializeNoises();
}

void TerrainGenerator::InitializeNoises()
{
    noises_.emplace(NoiseType::PERLIN, std::make_unique<PerlinNoise>(PerlinNoise{ seed_ }));
    noises_.emplace(NoiseType::SIMPLEX, std::make_unique<SimplexNoise>(SimplexNoise{ seed_ }));
    noises_.emplace(NoiseType::WORLEY, std::make_unique<WorleyNoise>(WorleyNoise{ seed_ }));
    noises_.emplace(NoiseType::OPEN_SIMPLEX_2F, std::make_unique<OpenSimplex2FNoise>(OpenSimplex2FNoise{ seed_ }));
    noises_.emplace(NoiseType::OPEN_SIMPLEX_2S, std::make_unique<OpenSimplex2SNoise>(OpenSimplex2SNoise{ seed_ }));
}

void TerrainGenerator::GenerateChunkHeightMapTerrain(const std::shared_ptr<Chunk>& chunk, std::stop_token stop_token)
{
	assert(chunk != nullptr);

	const double frequency = 0.01;
	const auto find_it = noises_.find(GetNoiseType());
	assert(find_it != noises_.end());

	const std::unique_ptr<Noise>& noise = find_it->second;
	const glm::ivec2 chunk_world_coords = chunk->WorldCoords();

	for (int z = 0; z < constants::chunk::depth; ++z)
	{
		for (int x = 0; x < constants::chunk::width; ++x)
		{
			const int worldX = chunk_world_coords.x * constants::chunk::width + x;
			const int worldZ = chunk_world_coords.y * constants::chunk::depth + z;

			const double nx = worldX * frequency;
			const double nz = worldZ * frequency;

			const double noise_sample = noise->Sample(nx, nz);
			const double normalized_noise_sample = (noise_sample + 1.0) / 2.0;
			constexpr double exponent = 2.15;
			const int height = std::clamp(std::pow(normalized_noise_sample, exponent) * constants::chunk::height, 0, 90);

			for (int y = 0; y < constants::chunk::height; ++y)
			{
				if (y == 0)
				{
					chunk->BlockAt({ x, y, z }).SetType(BlockType::Bedrock);
				}
				else if (y <= height)
				{
					chunk->BlockAt({ x, y, z }).SetType(BlockType::Grass);
				}
				else
				{
					chunk->BlockAt({ x, y, z }).SetType(BlockType::Air);
				}
			}
		}
	}

	if (stop_token.stop_requested())
	{
		chunk->SetTerrainState(TerrainState::Cancelled);
		return;
	}

	chunk->SetTerrainGenerated(true);
}

double TerrainGenerator::GetCoordHeight(double x, double y)
{
    // which frequency for stupid simple height map?
    
	return 0.0;
}