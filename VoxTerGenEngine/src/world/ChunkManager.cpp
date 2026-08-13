#include "mesh/MeshBuilder.hpp"

#include "threading/ThreadPool.hpp"
#include "threading/ThreadSafeQueue.hpp"

#include "utils/MathUtils.hpp"

#include "world/Chunk.hpp"
#include "world/ChunkEvents.hpp"
#include "world/ChunkManager.hpp"

#include <queue>
#include <cmath>
#include <ranges>
#include <mutex>
#include <shared_mutex>
#include <stop_token>
#include <memory>
#include <cassert>
#include <array>

ChunkManager::ChunkManager(ThreadPool& thread_pool) : 
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
	std::lock_guard<std::shared_mutex> lock(chunks_shared_mutex_);

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
			chunk_build_queue_.Push(chunk.get());

			chunks_.try_emplace(world_coords, std::move(chunk));
		}
	}

	for (auto& chunk : chunks_ | std::views::values)
	{
		FillChunkTmp(*chunk);
	}
}

void ChunkManager::Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue, const Camera& camera)
{
	MarkChunksForUnload(camera);
	LoadChunks(camera);
	BuildChunkMeshes(chunk_event_queue);
	UnloadChunks(chunk_event_queue);
}

void ChunkManager::MarkChunksForUnload(const Camera& camera)
{
	const glm::ivec2 current_chunk_coords = GetChunkCoords(camera.Pos());
	const glm::ivec2 prev_chunk_coords = GetChunkCoords(camera.PrevPos());

	if (prev_chunk_coords == current_chunk_coords)
	{
		return;
	}

	std::lock_guard<std::shared_mutex> lock(chunks_shared_mutex_);

	for (auto& [chunk_world_coords, chunk] : chunks_)
	{
		if (chunk_world_coords.x < current_chunk_coords.x - constants::chunk::default_radius ||
			chunk_world_coords.x > current_chunk_coords.x + constants::chunk::default_radius ||
			chunk_world_coords.y < current_chunk_coords.y - constants::chunk::default_radius ||
			chunk_world_coords.y > current_chunk_coords.y + constants::chunk::default_radius)
		{
			chunk->StopSource().request_stop();
			chunk->SetChunkState(ChunkState::PendingUnload);
			chunk->SetMeshState(MeshState::Cancelled);
		}
	}
}

void ChunkManager::LoadChunks(const Camera& camera)
{
	std::lock_guard<std::shared_mutex> lock(chunks_shared_mutex_);

	const glm::ivec2 current_chunk_coords = GetChunkCoords(camera.Pos());

	for (int y = current_chunk_coords.y - constants::chunk::default_radius; y < current_chunk_coords.y + constants::chunk::default_radius + 1; ++y)
	{
		for (int x = current_chunk_coords.x - constants::chunk::default_radius; x < current_chunk_coords.x + constants::chunk::default_radius + 1; ++x)
		{
			const glm::ivec2 chunk_world_coords = { x, y };

			if (chunks_.find(chunk_world_coords) == chunks_.end())
			{
				std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(next_chunk_id_++, chunk_world_coords);
				FillChunkTmp(*chunk);
				chunk_build_queue_.Push(chunk.get());
				chunk->SetChunkState(ChunkState::Loaded);
				chunks_.try_emplace(chunk_world_coords, std::move(chunk));
			}
		}
	}
}

void ChunkManager::UnloadChunks(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	std::lock_guard<std::shared_mutex> lock(chunks_shared_mutex_);

	auto it = chunks_.begin();

	while (it != chunks_.end())
	{
		Chunk& chunk = *(it->second);

		if (chunk.GetChunkState() == ChunkState::PendingUnload)
		{
			chunk.SetChunkState(ChunkState::Unloaded);
			chunk_event_queue.Push(chunk_event::ChunkDestroyed{ chunk.Id() });
			it = chunks_.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void ChunkManager::BuildChunkMeshes(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	constexpr int jobs_submitted_limit = 8;
	int jobs_submitted = 0;

	while (jobs_submitted < jobs_submitted_limit)
	{
		const std::optional<Chunk*> chunk_opt = chunk_build_queue_.TryPop();

		if (!chunk_opt.has_value())
		{
			return;
		}

		assert(chunk_opt.value() != nullptr);
		Chunk& chunk = *chunk_opt.value();

		if (chunk.StopSource().stop_requested() || chunk.GetChunkState() == ChunkState::Unloaded || chunk.GetMeshState() != MeshState::Invalid)
		{
			continue;
		}

		chunk.SetMeshState(MeshState::Building);
		
		const ChunkMeshDependencies chunk_mesh_dependencies = GetMeshDependencies(chunk.WorldCoords());

		thread_pool_.Enqueue([this, &chunk, &chunk_mesh_dependencies, &chunk_event_queue]()
		{
			std::unique_ptr<Mesh> chunk_mesh = BuildChunkMesh(chunk, chunk_mesh_dependencies, chunk.StopSource().get_token());

			if (chunk_mesh == nullptr)
			{
				chunk.SetChunkState(ChunkState::PendingUnload);
			}
			else
			{
				chunk_event_queue.Push(chunk_event::ChunkMeshReady{ chunk.Id(), chunk.WorldCoords(), std::move(chunk_mesh) });
				chunk.SetMeshState(MeshState::Ready);
			}
		});

		++jobs_submitted;
	}
}

std::unique_ptr<Mesh> ChunkManager::BuildChunkMesh(Chunk& chunk, const ChunkMeshDependencies& chunk_mesh_dependencies, std::stop_token stop_token)
{
	std::unique_ptr<Mesh> chunk_mesh = std::make_unique<Mesh>();

	const auto world_block_query = [this, &chunk](glm::ivec3 block_coords)
		{
			return WorldBlockQuery(chunk.WorldCoords(), block_coords);
		};
		
	*chunk_mesh = MeshBuilder::BuildMeshGreedy(world_block_query, stop_token);

	if (stop_token.stop_requested())
	{
		return nullptr;
	}

	return chunk_mesh;
}

std::shared_ptr<Chunk> ChunkManager::GetChunkAt(glm::ivec2 chunk_coord) const
{
	const auto& chunk_it = chunks_.find(chunk_coord);

	if (chunk_it == chunks_.end())
	{
		return nullptr;
	}

	return chunk_it->second;
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
		current_chunk_coord.y + z_chunk_offset 
	};

	const glm::ivec3 absolute_block_coords = { 
		target_block_coords.x + chunk_coords.x * constants::chunk::width, 
		target_block_coords.y, 
		target_block_coords.z + chunk_coords.y * constants::chunk::depth 
	};

	if (const std::shared_ptr<Chunk> target_chunk = GetChunkAt(chunk_coords))
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

ChunkMeshDependencies ChunkManager::GetMeshDependencies(glm::ivec2 chunk_coords) const
{
	ChunkMeshDependencies chunk_mesh_dependencies;

    std::shared_lock lock(chunks_shared_mutex_);

	for (int y_offset = -1; y_offset < 2; ++y_offset)
	{
		for (int x_offset = -1; x_offset < 2; ++x_offset)
		{
			chunk_mesh_dependencies.chunks_.push_back(GetChunkAt(chunk_coords + glm::ivec2{ x_offset, y_offset }));
		}	
	}

	return chunk_mesh_dependencies;
}