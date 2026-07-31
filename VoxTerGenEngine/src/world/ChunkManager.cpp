#include "mesh/MeshBuilder.hpp"

#include "threading/ThreadPool.hpp"

#include "utils/MathUtils.hpp"

#include "world/Chunk.hpp"
#include "world/ChunkEvents.hpp"
#include "world/ChunkManager.hpp"

#include <queue>
#include <cmath>
#include <ranges>
#include <memory>

ChunkManager::ChunkManager(const ThreadPool& thread_pool) : 
	thread_pool_(thread_pool)
{	
}

void ChunkManager::FillChunkTmp(Chunk& chunk)
{
	static int i = 1;

	//chunk.BlockAt({ 0, 0, 0 }).SetType(static_cast<BlockType>(i));
	//chunk.BlockAt({ 1, 0, 0 }).SetType(static_cast<BlockType>(i));
	////chunk.BlockAt({ 2, 2, 0 }).SetType(static_cast<BlockType>(1));

	//if (++i >= 8)
	//{
	//	i = 1;
	//}

	//return;

	for (int y = 0; y < constants::chunk::height; ++y)
	{
		for (int z = 0; z < constants::chunk::depth; ++z)
		{
			for (int x = 0; x < constants::chunk::width; ++x)
			{
				chunk.BlockAt({ x, i, z }).SetType(static_cast<BlockType>(i));
				//if (i >= 8)
				//{
				//	i = 1;
				//}
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
	assert(chunk_radius >= 0);
	const int chunk_square_size = 2 * chunk_radius + 1;
	// TODO: observer will not always start at 0, 0!
	const glm::ivec3 start_coords = { 0 - chunk_radius, 0, 0 - chunk_radius };

	for (int z = 0; z < chunk_square_size; ++z)
	{
		for (int x = 0; x < chunk_square_size; ++x)
		{
			const glm::ivec2 world_coords = { start_coords.x + x, start_coords.z + z };
			std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(next_chunk_id_++, world_coords);
			chunk_build_queue_.push(chunk.get());

			chunks_.try_emplace(world_coords, std::move(chunk));
		}
	}

	for (auto& chunk : chunks_ | std::views::values)
	{
		FillChunkTmp(*chunk);
	}
}

void ChunkManager::Tick(std::queue<ChunkEvent>& chunk_event_queue, const Camera& camera)
{
	LoadChunks(chunk_event_queue, camera);
	BuildChunkMeshes(chunk_event_queue);
}

void ChunkManager::LoadChunks(std::queue<ChunkEvent>& chunk_event_queue, const Camera& camera)
{
	const glm::ivec2 current_chunk_coords = GetChunkCoords(camera.Pos());
	const glm::ivec2 prev_chunk_coords = GetChunkCoords(camera.PrevPos());

	if (prev_chunk_coords == current_chunk_coords)
	{
		return;
	}

	auto it = chunks_.begin();

	while (it != chunks_.end())
	{
		const glm::ivec2& chunk_world_coords = it->first;

		if (chunk_world_coords.x < current_chunk_coords.x - constants::chunk::default_radius || 
			chunk_world_coords.x > current_chunk_coords.x + constants::chunk::default_radius ||
			chunk_world_coords.y < current_chunk_coords.y - constants::chunk::default_radius || 
			chunk_world_coords.y > current_chunk_coords.y + constants::chunk::default_radius)
		{
			chunk_event_queue.emplace(chunk_event::ChunkDestroyed{ it->second->Id() });
			it = chunks_.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (int y = current_chunk_coords.y - constants::chunk::default_radius; y < current_chunk_coords.y + constants::chunk::default_radius + 1; ++y)
	{
		for (int x = current_chunk_coords.x - constants::chunk::default_radius; x < current_chunk_coords.x + constants::chunk::default_radius + 1; ++x)
		{
			const glm::ivec2 chunk_world_coords = { x, y };

			if (chunks_.find(chunk_world_coords) == chunks_.end())
			{
				std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(next_chunk_id_++, chunk_world_coords);
				FillChunkTmp(*chunk);
				chunk_build_queue_.emplace(chunk.get());
				chunks_.try_emplace(chunk_world_coords, std::move(chunk));
			}
		}
	}
}

void ChunkManager::BuildChunkMeshes(std::queue<ChunkEvent>& chunk_event_queue)
{
	constexpr int meshes_build_limit = 1;
	int meshes_built = 0;

	while (!chunk_build_queue_.empty() && meshes_built < meshes_build_limit)
	{
		Chunk* chunk = chunk_build_queue_.front();
		chunk_build_queue_.pop();

		if (!chunk->MeshValid())
		{
			chunk_event_queue.emplace(chunk_event::ChunkMeshReady{ chunk->Id(), chunk->WorldCoords(), BuildChunkMesh(*chunk) });
			++meshes_built;
			chunk->SetMeshValid(true);
		}
	}
}

std::unique_ptr<Mesh> ChunkManager::BuildChunkMesh(Chunk& chunk)
{
	for (int y = 0; y < constants::chunk::height - 1; ++y)
	{
		for (int z = 0; z < constants::chunk::depth; ++z)
		{
			for (int x = 0; x < constants::chunk::width; ++x)
			{
				const glm::ivec3 coords = { x, y, z };
				Block& current_block = chunk.BlockAt(coords);
				const Block& block_above = chunk.NeighborRefAt(coords, Direction::PosY);

				if (current_block.Type() == BlockType::Grass && block_above.IsSolid())
				{
					current_block.SetType(BlockType::Dirt);
				}			
			}
		}
	}

	std::unique_ptr<Mesh> chunk_mesh = std::make_unique<Mesh>();
		
	*chunk_mesh = MeshBuilder::BuildMeshGreedy(
		[this, &chunk](glm::ivec3 block_coords)
		{
			return WorldBlockQuery(chunk.WorldCoords(), block_coords);
		}
	);

	return chunk_mesh;
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

glm::ivec3 ChunkManager::AbsoluteBlockPos(glm::dvec3 position, glm::dvec3 pos_offset) const noexcept
{
    return {
        static_cast<int>(std::floor(position.x + pos_offset.x)),
        static_cast<int>(std::floor(position.y + pos_offset.y)),
        static_cast<int>(std::floor(position.z + pos_offset.z))
    };
}

glm::ivec3 ChunkManager::RelativeBlockPos(glm::dvec3 position, glm::dvec3 pos_offset) const noexcept
{
	glm::ivec3 block_pos(0);

	if (position.x + pos_offset.x < 0.0)
	{
		const int chunk_x_offset = (std::abs(static_cast<int>(position.x + pos_offset.x)) / constants::chunk::width) + 1;
		block_pos.x = (static_cast<int>(position.x + pos_offset.x) + chunk_x_offset * constants::chunk::width) - 1;
	}
	else
	{
		block_pos.x = static_cast<int>(position.x + pos_offset.x) % constants::chunk::width;
	}

	block_pos.y = static_cast<int>(std::floor(position.y + pos_offset.y));

	if (position.z + pos_offset.z < 0.0)
	{
		const int chunk_z_offset = (std::abs(static_cast<int>(position.z + pos_offset.z)) / constants::chunk::depth) + 1;
		block_pos.z = (static_cast<int>(position.z + pos_offset.z) + chunk_z_offset * constants::chunk::depth) - 1;
	}
	else
	{
		block_pos.z = static_cast<int>(position.z + pos_offset.z) % constants::chunk::depth;
	}

	return block_pos;
}

BlockInfo ChunkManager::WorldBlockQuery(glm::ivec2 current_chunk_coord, glm::ivec3 block_coords) const
{	
	if (block_coords.y < 0 || block_coords.y >= constants::chunk::height)
	{
		return { Block(), glm::ivec3(0) };
	}
	
	const int x_chunk_offset = math_utils::FloorDiv(block_coords.x, constants::chunk::width);
	const int z_chunk_offset = math_utils::FloorDiv(block_coords.z, constants::chunk::depth);

	const glm::ivec3 target_block_coords = { 
		block_coords.x - (x_chunk_offset * constants::chunk::width), 
		block_coords.y, 
		block_coords.z - (z_chunk_offset * constants::chunk::depth) 
	};

	const glm::ivec2 chunk_coords = { 
		current_chunk_coord.x + x_chunk_offset, 
		current_chunk_coord.y + z_chunk_offset };

	const glm::ivec3 absolute_block_coords = { 
		target_block_coords.x + chunk_coords.x * constants::chunk::width, 
		target_block_coords.y, 
		target_block_coords.z + chunk_coords.y * constants::chunk::depth };
	
	if (const Chunk* target_chunk = GetChunkAt(chunk_coords))
	{
		return { target_chunk->BlockAt(target_block_coords), absolute_block_coords };
	}

	return { Block(), glm::ivec3(0) };
}

glm::ivec2 ChunkManager::GetChunkCoords(glm::dvec3 pos) const noexcept
{
	return { 
		static_cast<int>(std::floor(pos.x / constants::chunk::width)), 
		static_cast<int>(std::floor(pos.z / constants::chunk::depth)) 
	};
}
