#include "world/ChunkManager.hpp"
#include "mesh/MeshBuilder.hpp"
#include "world/Chunk.hpp"
#include "core/Settings.hpp"
#include "utils/Logger.hpp"

std::size_t ivec2_hash::operator()(const glm::ivec2& vec) const noexcept
{
	const std::size_t h1 = std::hash<int>{}(vec.x);
	const std::size_t h2 = std::hash<int>{}(vec.y);
	return h1 ^ (h2 << 1);
}

ChunkManager::ChunkManager()
{
}

#include <random>

void ChunkManager::InitChunks(int chunk_radius)
{
	const int chunk_square_size = 2 * chunk_radius + 1;
	const glm::ivec3 start_coords = { 0 - chunk_radius, 0, 0 - chunk_radius };

	for (int z = 0; z < chunk_square_size; ++z)
	{
		for (int x = 0; x < chunk_square_size; ++x)
		{
			const glm::ivec2 world_coords = { start_coords.x + x, start_coords.z + z };
			std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(world_coords);
			InitChunkBlocks(*chunk);
			chunks_.emplace(world_coords, std::move(chunk));
		}
	}

	//for (int y = 0; y < 4; ++y)
	//{
	//	for (int z = 0; z < 4; ++z)
	//	{
	//		for (int x = 0; x < 4; ++x)
	//		{
	//			int randNum = rand() % (static_cast<int>(BlockType::Bedrock) - static_cast<int>(BlockType::Air) + 1) + static_cast<int>(BlockType::Air);

	//			chunks_.begin()->second->BlockAt({ x, y, z }).SetType(static_cast<BlockType>(randNum));
	//		}
	//	}
	//}

	//for (int y = 0; y < constants::chunk::height; ++y)
	//{
	//	for (int z = 0; z < constants::chunk::depth; ++z)
	//	{
	//		for (int x = 0; x < constants::chunk::width; ++x)
	//		{
	//			int randNum = rand() % (static_cast<int>(BlockType::Bedrock) - static_cast<int>(BlockType::Air) + 1) + static_cast<int>(BlockType::Air);

	//			chunks_.begin()->second->BlockAt({ x, y, z }).SetType(static_cast<BlockType>(randNum));
	//		}
	//	}
	//}

	//chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 0 }).SetType(BlockType::Water);
	//chunks_.begin()->second->BlockAt({ 0, 2, 0 }).SetType(BlockType::Grass);
	//chunks_.begin()->second->BlockAt({ 0, 3, 0 }).SetType(BlockType::Sand);
	//chunks_.begin()->second->BlockAt({ 0, 4, 0 }).SetType(BlockType::Snow);
	//chunks_.begin()->second->BlockAt({ 0, 5, 0 }).SetType(BlockType::Bedrock);
	//chunks_.begin()->second->BlockAt({ 0, 6, 0 }).SetType(BlockType::Dirt);

	//chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Stone);
	chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Dirt);
	chunks_.begin()->second->BlockAt({ 0, 0, 1 }).SetType(BlockType::Dirt);


	BuildAllChunkMeshes();
}

void ChunkManager::InitChunkBlocks(Chunk& chunk)
{
	for (int y = 0; y < constants::chunk::height; ++y)
	{
		for (int z = 0; z < constants::chunk::depth; ++z)
		{
			for (int x = 0; x < constants::chunk::width; ++x)
			{
				Block& block = chunk.BlockAt({ x, y, z });
				block.SetType(BlockType::Air);
				block.SetSunLight(0);
				block.SetBlockLight(0);
			}
		}
	}
}

void ChunkManager::BuildAllChunkMeshes()
{
	for (auto& [world_coord, chunk] : chunks_)
	{
		if (!chunk->MeshInvalid())
		{
			continue;
		}

		std::unique_ptr<Mesh> chunk_mesh = std::make_unique<Mesh>();
		
		*chunk_mesh = MeshBuilder::BuildMeshGreedy(
			[this, &chunk](const glm::ivec3& block_coords)
			{
				return WorldBlockQuery(chunk->WorldCoords(), block_coords);
			}
		);

		//*chunk_mesh = MeshBuilder::BuildMeshNaive(chunk->WorldCoords(),
		//	[this, &chunk](const glm::ivec3& block_coords)
		//	{
		//		return WorldBlockQuery(chunk->WorldCoords(), block_coords);
		//	}
		//);

		chunk->SetMesh(std::move(chunk_mesh));
		chunk->SetMeshInvalid(false);
	}
}

const Chunk* ChunkManager::GetChunkAt(glm::ivec2 chunk_coord) const
{
	const auto chunk_it = chunks_.find(chunk_coord);

	if (chunk_it == chunks_.end())
	{
		return nullptr;
	}

	return chunk_it->second.get();
}

Block ChunkManager::WorldBlockQuery(glm::ivec2 current_chunk_coord, const glm::ivec3& block_coords) const
{
	if (block_coords.y < 0 || block_coords.y > constants::chunk::height - 1)
	{
		return Block();
	}

	const int x_chunk_offset = block_coords.x / constants::chunk::width;
	const int z_chunk_offset = block_coords.z / constants::chunk::depth;
	const Chunk* const target_chunk = GetChunkAt({ current_chunk_coord.x + x_chunk_offset, current_chunk_coord.y + z_chunk_offset });

	if (!target_chunk)
	{
		return Block();
	}
	
	int x_block_offset = block_coords.x % constants::chunk::width;
	int z_block_offset = block_coords.z % constants::chunk::depth;

	if (x_block_offset < 0)
	{
		x_block_offset += constants::chunk::width;
	}

	if (z_block_offset < 0)
	{
		z_block_offset += constants::chunk::depth;
	}
	
	return target_chunk->BlockAt({ x_block_offset, block_coords.y, z_block_offset });
}