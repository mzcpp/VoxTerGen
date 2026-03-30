#include "world/ChunkManager.hpp"
#include "mesh/MeshBuilder.hpp"
#include "world/Chunk.hpp"
#include "core/Settings.hpp"
#include "utils/Logger.hpp"
#include "utils/MathUtils.hpp"
#include "utils/Hash.hpp"

#include <cmath>
#include <ranges>

ChunkManager::ChunkManager()
{
}

void ChunkManager::FillChunkTmp(const Chunk& chunk)
{
	static int i = 1;

	for (int y = 0; y < constants::chunk::height; ++y)
	{
		for (int z = 0; z < constants::chunk::depth; ++z)
		{
			for (int x = 0; x < constants::chunk::width; ++x)
			{
				BlockType block_type = static_cast<BlockType>(i);
				chunk->BlockAt({ x, 0, z }).SetType(randBlock);
			}
		}
	}

	if (++i >= 8)
	{
		i = 1;
	}
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
			chunk_build_queue_.push(chunk.get());
			chunks_.emplace(world_coords, std::move(chunk));
		}
	}

	//chunks_.at({ 0, 0 })->BlockAt({ 0, 50, 0 }).SetType(BlockType::Water);
	//chunks_.at({ -1, 0 })->BlockAt({ 15, 50, 0 }).SetType(BlockType::Grass);
	//chunks_.at({ 0, -1 })->BlockAt({ 0, 50, 15 }).SetType(BlockType::Bedrock);

	//for (auto& [world_coords, chunk] : chunks_)
	//{
	//	//BlockType randBlock = static_cast<BlockType>(rand() % (static_cast<int>(BlockType::Bedrock) - static_cast<int>(BlockType::Grass) + 1) + static_cast<int>(BlockType::Grass));

	//	for (int y = 0; y < constants::chunk::height; ++y)
	//	{
	//		for (int z = 0; z < constants::chunk::depth; ++z)
	//		{
	//			for (int x = 0; x < constants::chunk::width; ++x)
	//			{
	//				BlockType randBlock = static_cast<BlockType>(rand() % (static_cast<int>(BlockType::Dirt) - static_cast<int>(BlockType::Grass) + 1) + static_cast<int>(BlockType::Grass));
	//				chunk->BlockAt({ x, 0, z }).SetType(randBlock);

	//				/*chunks_.at({ -1, 1 })->BlockAt({ x, y, z }).SetType(BlockType::Grass);
	//				chunks_.at({ 0, 1 })->BlockAt({ x, y, z }).SetType(BlockType::Dirt);
	//				chunks_.at({ 1, 1 })->BlockAt({ x, y, z }).SetType(BlockType::Stone);

	//				chunks_.at({ -1, 0 })->BlockAt({ x, y, z }).SetType(BlockType::Snow);
	//				chunks_.at({ 0, 0 })->BlockAt({ x, y, z }).SetType(BlockType::Bedrock);
	//				chunks_.at({ 1, 0 })->BlockAt({ x, y, z }).SetType(BlockType::Sand);

	//				chunks_.at({ -1, -1 })->BlockAt({ x, y, z }).SetType(BlockType::Water);
	//				chunks_.at({ 0, -1 })->BlockAt({ x, y, z }).SetType(BlockType::Grass);
	//				chunks_.at({ 1, -1 })->BlockAt({ x, y, z }).SetType(BlockType::Bedrock);*/

	//				//int posy = y;

	//				//chunks_.at({ -1, 1 })->BlockAt({ x, posy, z }).SetType(randBlock);
	//				//chunks_.at({ 0, 1 })->BlockAt({ x, posy, z }).SetType(randBlock);
	//				//chunks_.at({ 1, 1 })->BlockAt({ x, posy, z }).SetType(randBlock);

	//				//chunks_.at({ -1, 0 })->BlockAt({ x, posy, z }).SetType(randBlock);
	//				//chunks_.at({ 0, 0 })->BlockAt({ x, posy, z }).SetType(randBlock);
	//				//chunks_.at({ 1, 0 })->BlockAt({ x, posy, z }).SetType(randBlock);

	//				//chunks_.at({ -1, -1 })->BlockAt({ x, posy, z }).SetType(randBlock);
	//				//chunks_.at({ 0, -1 })->BlockAt({ x, posy, z }).SetType(randBlock);
	//				//chunks_.at({ 1, -1 })->BlockAt({ x, posy, z }).SetType(randBlock);

	//			}
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

void ChunkManager::Tick(const Camera& camera)
{
	StreamChunks(camera);
	BuildChunkMeshes();
}

void ChunkManager::StreamChunks(const Camera& camera)
{
	const glm::ivec2 current_chunk_coords = { static_cast<int>(camera.Pos().x) / constants::chunk::width, static_cast<int>(camera.Pos().z) / constants::chunk::depth };
	const glm::ivec2 prev_chunk_coords = { static_cast<int>(camera.PrevPos().x) / constants::chunk::width, static_cast<int>(camera.PrevPos().z) / constants::chunk::depth };

	if (prev_chunk_coords == current_chunk_coord)
	{
		return;
	}

	std::cout << "CHANGED CHUNK POSITION!!\n";

	for (int i = 0; i < (radius * 2) + 1; ++i)
	{ 
		// MERGE THIS !!! NO IF ELSE !!!!
		// IN EACH CASE AFTER UNLOAD AND LOAD, 
		// FILL IT UP WITH TMP FUNCTION
		// PUSH TO QUEUE TO REBUILD MESH!
		if (current_chunk_coord.x > prev_chunk_coords.x)
		{
			// UNLOAD LEFT
			// UNLOAD (prev_chunk_coord.x - radius, prev_chunk_coord.y - radius + i) IF IT STILL THERE
			// LOAD (prev_chunk_coord.x + radius + 1, prev_chunk_coord.y - radius + i) IF NOT ALREADY THERE
			// std::cout << "UNLOADED LEFT!\n";
		}
		else
		{
			// UNLOAD RIGHT
			// UNLOAD (prev_chunk_coord.x + radius, prev_chunk_coord.y - radius + i) IF IT STILL THERE
			// LOAD (prev_chunk_coord.x - radius - 1, prev_chunk_coord.y - radius + i) IF NOT ALREADY THERE
			// std::cout << "UNLOADED RIGHT!\n";
		}

		if (current_chunk_coord.z > prev_chunk_coords.z)
		{
			// UNLOAD UP
			// UNLOAD (prev_chunk_coord.x - radius + i, prev_chunk_coord.y - radius) IF IT STILL THERE
			// LOAD (prev_chunk_coord.x - radius + i, prev_chunk_coord.y + radius + 1) IF NOT ALREADY THERE
			// std::cout << "UNLOADED UP!\n";
		}
		else
		{
			// UNLOAD DOWN
			// UNLOAD (prev_chunk_coord.x - radius + i, prev_chunk_coord.y + radius) IF IT STILL THERE
			// LOAD (prev_chunk_coord.x - radius + i, prev_chunk_coord.y - radius - 1) IF NOT ALREADY THERE
			// std::cout << "UNLOADED DOWN!\n";
		}
	}
}

void ChunkManager::BuildChunkMeshes()
{
	constexpr int meshes_build_limit = 1;
	int meshes_built = 0;

	while (!chunk_build_queue_.empty() && meshes_built < meshes_build_limit)
	{
		Chunk* chunk = chunk_build_queue_.front();
		chunk_build_queue_.pop();

		if (!chunk->MeshValid())
		{
			BuildChunkMesh(*chunk);
			++meshes_built;
		}
	}
}

void ChunkManager::BuildChunkMesh(Chunk& chunk)
{
	std::unique_ptr<Mesh> chunk_mesh = std::make_unique<Mesh>();
		
	*chunk_mesh = MeshBuilder::BuildMeshGreedy(
		[this, &chunk](const glm::ivec3& block_coords)
		{
			return WorldBlockQuery(chunk.WorldCoords(), block_coords);
		}
	);

	//*chunk_mesh = MeshBuilder::BuildMeshNaive(chunk->WorldCoords(),
	//	[this, &chunk](const glm::ivec3& block_coords)
	//	{
	//		return WorldBlockQuery(chunk->WorldCoords(), block_coords);
	//	}
	//);

	chunk.SetMesh(std::move(chunk_mesh));
	chunk.SetMeshValid(true);
	chunk.SetMeshNeedsUpload(true);
}

const Chunk* ChunkManager::GetChunkAt(glm::ivec2 chunk_coord) const
{
	const auto& chunk_it = chunks_.find(chunk_coord);

	if (chunk_it == chunks_.end())
	{
		return nullptr;
	}

	return chunk_it->second.get();
}

Block ChunkManager::WorldBlockQuery(const glm::ivec2& current_chunk_coord, const glm::ivec3& block_coords) const
{	
	if (block_coords.y < 0 || block_coords.y >= constants::chunk::height)
	{
		return Block();
	}
	
	const int x_chunk_offset = FloorDiv(block_coords.x, constants::chunk::width);
	const int z_chunk_offset = FloorDiv(block_coords.z, constants::chunk::depth);
	const glm::ivec3 target_block_coords = 
	{ 
		block_coords.x - (x_chunk_offset * constants::chunk::width), 
		block_coords.y, 
		block_coords.z - (z_chunk_offset * constants::chunk::depth) 
	};
	
	if (const Chunk* target_chunk = GetChunkAt({ current_chunk_coord.x + x_chunk_offset, current_chunk_coord.y + z_chunk_offset }))
	{
		return target_chunk->BlockAt(target_block_coords);
	}

	return Block();
}

void ChunkManager::PushChunkIntoQueue(Chunk* chunk)
{
	chunk_build_queue_.push(chunk);
}
