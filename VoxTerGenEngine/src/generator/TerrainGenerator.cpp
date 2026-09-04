#include "generator/TerrainGenerator.hpp"

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"
#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2FNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2SNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/PerlinNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/SimplexNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/WorleyNoise.hpp"

#include "world/Chunk.hpp"

#include <memory>
#include <unordered_map>

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

void TerrainGenerator::GenerateChunkTerrain(Chunk& chunk)
{
	double frequency = 0.01;

	for (int z = 0; z < constants::chunk::depth; ++z)
	{
		for (int x = 0; x < constants::chunk::width; ++x)
		{
			int worldX = chunk.WorldCoords().x * constants::chunk::width + x;
			int worldZ = chunk.WorldCoords().y * constants::chunk::depth + z;

			double nx = worldX * frequency;
			double nz = worldZ * frequency;

			double noise = noises_.find(NoiseType::PERLIN)->second->Sample(nx, nz);

			int height = static_cast<int>((noise * 0.5 + 0.5) * 40) + 20;

			for (int y = 0; y < constants::chunk::height; ++y)
			{
				if (y <= height)
				{
					chunk.BlockAt({ x, y, z }).SetType(BlockType::Grass);
				}
				else
				{
					chunk.BlockAt({ x, y, z }).SetType(BlockType::Air);
				}
			}
		}
	}
}

double TerrainGenerator::GetCoordHeight(double x, double y)
{
    // which frequency for stupid simple height map?
    
	return 0.0;
}