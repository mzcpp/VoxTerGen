#include "world/ChunkManager.hpp"
#include "mesh/MeshBuilder.hpp"
#include "world/Chunk.hpp"
#include "core/Settings.hpp"
#include "utils/Logger.hpp"
#include "utils/MathUtils.hpp"
#include "utils/Hash.hpp"
#include "graphics/Camera.hpp"

#include <queue>
#include <cmath>
#include <ranges>

ChunkManager::ChunkManager()
{
}

void ChunkManager::FillChunkTmp(Chunk& chunk)
{
	static int i = 1;

	for (int y = 0; y < constants::chunk::height; ++y)
	{
		for (int z = 0; z < constants::chunk::depth; ++z)
		{
			for (int x = 0; x < constants::chunk::width; ++x)
			{
				chunk.BlockAt({ x, 0, z }).SetType(static_cast<BlockType>(i));
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
	StreamChunks(chunk_event_queue, camera);
	BuildChunkMeshes(chunk_event_queue);
}

void ChunkManager::StreamChunks(std::queue<ChunkEvent>& chunk_event_queue, const Camera& camera)
{
	const glm::ivec2 current_chunk_coords = GetChunkCoords(camera.Pos());
	const glm::ivec2 prev_chunk_coords = GetChunkCoords(camera.PrevPos());

	if (prev_chunk_coords == current_chunk_coords)
	{
		return;
	}

	for (int i = 0; i < (constants::chunk::default_radius * 2) + 1; ++i)
	{ 
		glm::ivec2 to_erase_coords(0);
		glm::ivec2 to_emplace_coords(0);
	
		if (current_chunk_coords.x > prev_chunk_coords.x)
		{
			to_erase_coords = { prev_chunk_coords.x - constants::chunk::default_radius, prev_chunk_coords.y - constants::chunk::default_radius + i };
			to_emplace_coords = { prev_chunk_coords.x + constants::chunk::default_radius + 1, prev_chunk_coords.y - constants::chunk::default_radius + i };
		}
		else if (current_chunk_coords.x < prev_chunk_coords.x)
		{
			to_erase_coords = { prev_chunk_coords.x + constants::chunk::default_radius, prev_chunk_coords.y - constants::chunk::default_radius + i };
			to_emplace_coords = { prev_chunk_coords.x - constants::chunk::default_radius - 1, prev_chunk_coords.y - constants::chunk::default_radius + i };
		}

		if (current_chunk_coords.y > prev_chunk_coords.y)
		{
			to_erase_coords = { prev_chunk_coords.x - constants::chunk::default_radius + i, prev_chunk_coords.y - constants::chunk::default_radius };
			to_emplace_coords = { prev_chunk_coords.x - constants::chunk::default_radius + i, prev_chunk_coords.y + constants::chunk::default_radius + 1 };
		}
		else if (current_chunk_coords.y < prev_chunk_coords.y)
		{
			to_erase_coords = { prev_chunk_coords.x - constants::chunk::default_radius + i, prev_chunk_coords.y + constants::chunk::default_radius };
			to_emplace_coords = { prev_chunk_coords.x - constants::chunk::default_radius + i, prev_chunk_coords.y - constants::chunk::default_radius - 1 };
		}

		const auto& chunk_to_erase_it = chunks_.find(to_erase_coords);
		
		if (chunk_to_erase_it != chunks_.end())
		{
			chunk_event_queue.emplace(ChunkDestroyed{ chunk_to_erase_it->second->Id() });
			chunks_.erase(chunk_to_erase_it);
			std::cout << "ChunkDestroyed EVENT!\n";
		}	

		std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(next_chunk_id_++, to_emplace_coords);
		FillChunkTmp(*chunk);
		chunk_build_queue_.emplace(chunk.get());
		// what if this fails?
		chunks_.try_emplace(to_emplace_coords, std::move(chunk));
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
			chunk_event_queue.emplace(ChunkMeshReady{ chunk->Id(), BuildChunkMesh(*chunk) });
			std::cout << "ChunkMeshReady EVENT!\n";
			++meshes_built;
			chunk->SetMeshValid(true);
		}
	}
}

std::unique_ptr<Mesh> ChunkManager::BuildChunkMesh(Chunk& chunk)
{
	std::unique_ptr<Mesh> chunk_mesh = std::make_unique<Mesh>();
		
	*chunk_mesh = MeshBuilder::BuildMeshGreedy(
		[this, &chunk](const glm::ivec3& block_coords)
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

glm::ivec2 ChunkManager::GetChunkCoords(const glm::dvec3& pos) noexcept
{
	return { static_cast<int>(std::floor(pos.x / constants::chunk::width)), static_cast<int>(std::floor(pos.z / constants::chunk::depth)) };
}
