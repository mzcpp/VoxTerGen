#include "world/ChunkManager.hpp"

#include "mesh/MeshBuilder.hpp"

#include "render/events/ChunkEvents.hpp"

#include "threading/ThreadPool.hpp"
#include "threading/ThreadSafeQueue.hpp"

#include "utils/MathUtils.hpp"

#include "world/Chunk.hpp"

#include <cmath>
#include <ranges>
#include <shared_mutex>
#include <stop_token>
#include <memory>
#include <cassert>

ChunkManager::ChunkManager(Observer& observer, ThreadPool& thread_pool) :
	observer_(observer), 
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
				chunk.BlockAt({ x, 0, z }).SetType(static_cast<BlockType>(i));
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
	const glm::ivec2 observer_chunk_coords = GetChunkCoords(observer_.Pos());

	const glm::ivec2 start_coords = {
		observer_chunk_coords.x - chunk_radius,
		observer_chunk_coords.y - chunk_radius
	};

	for (int z = 0; z < chunk_square_size; ++z)
	{
		for (int x = 0; x < chunk_square_size; ++x)
		{
			const glm::ivec2 chunk_world_coords = { start_coords.x + x, start_coords.y + z };
			const double distance_squared = ChunkDistanceSquared(observer_chunk_coords, chunk_world_coords);
			std::shared_ptr<Chunk> chunk = std::make_unique<Chunk>(next_chunk_id_++, chunk_world_coords);
			chunk_build_queue_.Push(ChunkJob{ chunk, distance_squared });

			chunks_.try_emplace(chunk_world_coords, std::move(chunk));
		}
	}

	for (auto& chunk : chunks_ | std::views::values)
	{
		FillChunkTmp(*chunk);
	}
}

void ChunkManager::Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	MarkChunksForUnload();
	LoadChunks();
	BuildChunkMeshes(chunk_event_queue);
	UnloadChunks(chunk_event_queue);
}

void ChunkManager::MarkChunksForUnload()
{
	const glm::ivec2 observer_chunk_coords = GetChunkCoords(observer_.Pos());
	const glm::ivec2 observer_prev_chunk_coords = GetChunkCoords(observer_.PrevPos());

	if (observer_prev_chunk_coords == observer_chunk_coords)
	{
		return;
	}

	std::lock_guard<std::shared_mutex> lock(chunks_shared_mutex_);

	for (auto& [chunk_world_coords, chunk] : chunks_)
	{
		if (chunk_world_coords.x < observer_chunk_coords.x - constants::chunk::default_radius ||
			chunk_world_coords.x > observer_chunk_coords.x + constants::chunk::default_radius ||
			chunk_world_coords.y < observer_chunk_coords.y - constants::chunk::default_radius ||
			chunk_world_coords.y > observer_chunk_coords.y + constants::chunk::default_radius)
		{
			chunk->StopSource().request_stop();
			chunk->SetChunkState(ChunkState::PendingUnload);
			chunk->SetMeshState(MeshState::Cancelled);
		}
	}
}

void ChunkManager::LoadChunks()
{
	const glm::ivec2 observer_chunk_coords = GetChunkCoords(observer_.Pos());
	
	std::lock_guard<std::shared_mutex> lock(chunks_shared_mutex_);

	for (int y = observer_chunk_coords.y - constants::chunk::default_radius; y < observer_chunk_coords.y + constants::chunk::default_radius + 1; ++y)
	{
		for (int x = observer_chunk_coords.x - constants::chunk::default_radius; x < observer_chunk_coords.x + constants::chunk::default_radius + 1; ++x)
		{
			const glm::ivec2 chunk_world_coords = { x, y };

			if (chunks_.find(chunk_world_coords) == chunks_.end())
			{
				std::shared_ptr<Chunk> chunk = std::make_unique<Chunk>(next_chunk_id_++, chunk_world_coords);
				
				FillChunkTmp(*chunk);

				chunk_build_queue_.Push(ChunkJob{ chunk, ChunkDistanceSquared(observer_chunk_coords, chunk_world_coords) });
				chunk->SetChunkState(ChunkState::Loaded);
				chunk->SetMeshState(MeshState::Invalid);

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
			chunk_event_queue.Push(ChunkDestroyed{ chunk.Id() });
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
	constexpr int jobs_submitted_limit = 2048;
	int jobs_submitted = 0;

	while (jobs_submitted < jobs_submitted_limit)
	{
		const std::optional<ChunkJob> chunk_job_opt = chunk_build_queue_.TryPop();

		if (!chunk_job_opt.has_value())
		{
			return;
		}

		assert(chunk_job_opt->chunk_ != nullptr);
		const std::shared_ptr<Chunk> chunk = chunk_job_opt.value().chunk_;

		if (chunk->StopSource().stop_requested() || chunk->GetMeshState() != MeshState::Invalid)
		{
			continue;
		}

		chunk->SetMeshState(MeshState::Building);
		
		thread_pool_.Enqueue([this, chunk, chunk_mesh_dependencies = GetMeshDependencies(chunk->WorldCoords()), &chunk_event_queue]()
		{
			std::unique_ptr<ChunkMesh> chunk_mesh = BuildChunkMesh(chunk_mesh_dependencies, chunk->StopSource().get_token());

			if (chunk_mesh == nullptr)
			{
				chunk->SetChunkState(ChunkState::PendingUnload);
			}
			else
			{
				chunk_event_queue.Push(ChunkMeshReady{ chunk->Id(), chunk->WorldCoords(), std::move(chunk_mesh) });
				chunk->SetMeshState(MeshState::Ready);
			}
		});

		++jobs_submitted;
	}
}

std::unique_ptr<ChunkMesh> ChunkManager::BuildChunkMesh(const ChunkMeshDependencies& chunk_mesh_dependencies, std::stop_token stop_token)
{
	std::unique_ptr<ChunkMesh> chunk_mesh = std::make_unique<ChunkMesh>(MeshBuilder::BuildChunkMeshGreedy(chunk_mesh_dependencies, stop_token));
		
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
    return glm::ivec3{ std::floor(position.x + pos_offset.x), std::floor(position.y + pos_offset.y), std::floor(position.z + pos_offset.z) };
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
	return glm::ivec2{ std::floor(pos.x / constants::chunk::width), std::floor(pos.z / constants::chunk::depth) };
}

double ChunkManager::ChunkDistanceSquared(glm::ivec2 first_chunk, glm::ivec2 second_chunk) const noexcept
{
	const double dx = first_chunk.x - second_chunk.x;
	const double dy = first_chunk.y - second_chunk.y;

	return dx * dx + dy * dy;
}

ChunkMeshDependencies ChunkManager::GetMeshDependencies(glm::ivec2 chunk_coords) const
{
	ChunkMeshDependencies chunk_mesh_dependencies;

    std::shared_lock lock(chunks_shared_mutex_);

	int index = 0;

	for (int y_offset = -1; y_offset < 2; ++y_offset)
	{
		for (int x_offset = -1; x_offset < 2; ++x_offset)
		{
			chunk_mesh_dependencies.chunks_[index++] = GetChunkAt(chunk_coords + glm::ivec2{ x_offset, y_offset });
		}	
	}

	return chunk_mesh_dependencies;
}