#include "world/ChunkManager.hpp"
#include "mesh/MeshBuilder.hpp"
#include "world/Chunk.hpp"
#include "core/Settings.hpp"
#include "utils/Logger.hpp"
#include "utils/MathUtils.hpp"

#include <cmath>

std::size_t ivec2_hash::operator()(const glm::ivec2& vec) const noexcept
{
	const std::size_t h1 = std::hash<int>{}(vec.x);
	const std::size_t h2 = std::hash<int>{}(vec.y);
	return h1 ^ (h2 << 1);
}

ChunkManager::ChunkManager()
{
}

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

	chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Stone);
	chunks_.begin()->second->BlockAt({ 0, 0, 1 }).SetType(BlockType::Stone);

	//for (int y = 0; y < constants::chunk::height; ++y)
	//{
	//	for (int z = 0; z < constants::chunk::depth; ++z)
	//	{
	//		for (int x = 0; x < constants::chunk::width; ++x)
	//		{
	//			int randNum = rand() % (static_cast<int>(BlockType::Bedrock) - static_cast<int>(BlockType::Air) + 1) + static_cast<int>(BlockType::Air);

	//			chunks_.begin()->second->BlockAt({ x, 0, z }).SetType(static_cast<BlockType>(randNum));
	//		}
	//	}
	//}

	//chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Grass);
	//chunks_.begin()->second->BlockAt({ 0, 0, 1 }).SetType(BlockType::Grass);
	//chunks_.begin()->second->BlockAt({ 0, 0, 2 }).SetType(BlockType::Grass);
	//chunks_.begin()->second->BlockAt({ 0, 0, 3 }).SetType(BlockType::Grass);
	//chunks_.begin()->second->BlockAt({ 0, 1, 2 }).SetType(BlockType::Grass);

	//chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Dirt);
	//chunks_.begin()->second->BlockAt({ 0, 0, 1 }).SetType(BlockType::Dirt);

	//chunks_.begin()->second->BlockAt({ 1, 0, 1 }).SetType(BlockType::Water);
	//chunks_.begin()->second->BlockAt({ 1, 0, 2 }).SetType(BlockType::Dirt);
	//chunks_.begin()->second->BlockAt({ 0, 0, 1 }).SetType(BlockType::Grass);
	//chunks_.begin()->second->BlockAt({ 2, 0, 1 }).SetType(BlockType::Bedrock);
	//chunks_.begin()->second->BlockAt({ 2, 0, 1 }).SetType(BlockType::Bedrock);

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

Block ChunkManager::WorldBlockQuery(const glm::ivec2& current_chunk_coord, const glm::ivec3& block_coords) const
{	
	if (block_coords.y < 0 || block_coords.y > constants::chunk::height - 1)
	{
		return Block();
	}
	
	const int x_chunk_offset = RoundAwayFromZero(static_cast<float>(block_coords.x) / static_cast<float>(constants::chunk::width));
	const int z_chunk_offset = RoundAwayFromZero(static_cast<float>(block_coords.z) / static_cast<float>(constants::chunk::depth));
	const glm::ivec2 target_chunk_coords = { current_chunk_coord.x + x_chunk_offset, current_chunk_coord.y + z_chunk_offset };

	glm::ivec3 target_block_coords = block_coords;

	if (block_coords.x < 0)
	{
		target_block_coords.x += constants::chunk::width * x_chunk_offset;
	}
	else if (block_coords.x > constants::chunk::width - 1)
	{
		target_block_coords.x = block_coords.x % constants::chunk::width;
	}
	
	if (block_coords.z < 0)
	{
		target_block_coords.z += constants::chunk::depth * z_chunk_offset;
	}
	else if (block_coords.z > constants::chunk::depth - 1)
	{
		target_block_coords.z = block_coords.z % constants::chunk::depth;
	}
	
	if (const Chunk* target_chunk = GetChunkAt(target_chunk_coords))
	{
		return target_chunk->BlockAt(target_block_coords);
	}

	return Block();
}