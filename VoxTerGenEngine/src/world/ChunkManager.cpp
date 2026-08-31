#include "world/ChunkManager.hpp"

#include "mesh/MeshBuilder.hpp"

#include "render/events/ChunkEvents.hpp"

#include "threading/ThreadPool.hpp"
#include "threading/ThreadSafeQueue.hpp"

#include "utils/MathUtils.hpp"

#include "world/Chunk.hpp"

#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

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
			std::shared_ptr<Chunk> chunk = std::make_unique<Chunk>(next_chunk_id_++, chunk_world_coords);

			FillChunkTmp(*chunk);

			current_chunk->IncrementMeshId();

			chunk_build_queue_.Push(
				ChunkJob{ 
					chunk, 
					current_chunk->MeshId(), 
					ChunkDistanceSquared(observer_chunk_coords, chunk_world_coords), 
					chunk->StopSource().get_token() 
				}
			);

			chunks_.try_emplace(chunk_world_coords, std::move(chunk));
		}
	}
}

void ChunkManager::Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	MarkChunksForUnload();
	DetermineChunksCoordsForLoad();
	
	UnloadChunks(chunk_event_queue);
	LoadChunks();

	BuildChunkMeshes(chunk_event_queue);

	// InitChunks is really ok??
	// token into worker
}

// TODO rename
void ChunkManager::MarkChunksForUnload()
{
	chunks_to_unload_.clear();

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
			chunk->StopSource() = std::stop_source{};

			chunk->SetChunkState(ChunkState::PendingUnload);
			chunk->SetMeshState(MeshState::Cancelled);

			chunks_to_unload_.push_back(chunk_world_coords);
		}
	}
}

// TODO rename
void ChunkManager::DetermineChunksCoordsForLoad()
{
	chunks_to_load_.clear();

	const glm::ivec2 observer_chunk_coords = GetChunkCoords(observer_.Pos());

	std::lock_guard<std::shared_mutex> lock(chunks_shared_mutex_);

	for (int y = observer_chunk_coords.y - constants::chunk::default_radius; y < observer_chunk_coords.y + constants::chunk::default_radius + 1; ++y)
	{
		for (int x = observer_chunk_coords.x - constants::chunk::default_radius; x < observer_chunk_coords.x + constants::chunk::default_radius + 1; ++x)
		{
			const glm::ivec2 chunk_world_coords = { x, y };

			if (chunks_.find(chunk_world_coords) != chunks_.end())
			{
				continue;
			}

			chunks_to_load_.emplace_back(x, y);
		}
	}
}

void ChunkManager::UnloadChunks(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	std::lock_guard<std::shared_mutex> lock(chunks_shared_mutex_);

	auto it = chunks_.begin();

	while (it != chunks_.end())
	{
		assert(it->second != nullptr);
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

void ChunkManager::LoadChunks()
{
	const glm::ivec2 observer_chunk_coords = GetChunkCoords(observer_.Pos());

	std::lock_guard<std::shared_mutex> lock(chunks_shared_mutex_);

	for (glm::ivec2 chunk_coords : chunks_to_load_)
	{
		std::shared_ptr<Chunk> chunk = std::make_unique<Chunk>(next_chunk_id_++, chunk_coords);

		FillChunkTmp(*chunk);

		// TODO REMOVE LATER
		chunk_build_queue_.Push(ChunkJob{ chunk, 0, ChunkDistanceSquared(observer_chunk_coords, chunk_coords), chunk->StopSource().get_token() });
		// TODO REMOVE LATER

		chunk->SetChunkState(ChunkState::Loaded);
		chunk->SetMeshState(MeshState::Invalid);

		chunks_.try_emplace(chunk_coords, std::move(chunk));
	}
}

void ChunkManager::DetermineChunksMeshesToBuild()
{
	const glm::ivec2 observer_chunk_coords = GetChunkCoords(observer_.Pos());
	
	std::lock_guard<std::shared_mutex> lock(chunks_shared_mutex_);

	for (glm::ivec2 chunk_world_coords : chunks_to_load_)
	{
		std::shared_ptr<Chunk> current_chunk = GetChunkAt(chunk_world_coords);

		if (current_chunk == nullptr)
		{
			continue;
		}

		current_chunk->IncrementMeshId();

		chunk_build_queue_.Push(
			ChunkJob{ 
				current_chunk, 
				current_chunk->MeshId();
				ChunkDistanceSquared(observer_chunk_coords, chunk_world_coords), 
				current_chunk->StopSource().get_token() 
			}
		);

		EnqueueNeighborChunkMeshesBuild(observer_chunk_coords, chunk_world_coords);
	}

	for (glm::ivec2 chunk_world_coords : chunks_to_unload_)
	{
		EnqueueNeighborChunkMeshesBuild(observer_chunk_coords, chunk_world_coords);
	}

}

void ChunkManager::EnqueueNeighborChunkMeshesBuild(glm::ivec2 observer_chunk_coords, glm::ivec2 chunk_world_coords)
{
	for (int i : { -1, 1 })
	{
		for (int j = 0; j < 4; ++j)
		{
			glm::ivec2 offset = { i, 0 };

			if (j > 1)
			{
				std::swap(offset.x, offset.y);
			}

			std::shared_ptr<Chunk> neighbor_chunk = GetChunkAt(chunk_world_coords + offset);

			if (neighbor_chunk == nullptr)
			{
				continue;
			}

			neighbor_chunk->StopSource().request_stop();
			neighbor_chunk->StopSource() = std::stop_source{};
			neighbor_chunk->SetMeshState(MeshState::Invalid);

			neighbor_chunk->IncrementMeshId();

			chunk_build_queue_.Push(
				ChunkJob{
					neighbor_chunk, 
					neighbor_chunk->MeshId();
					ChunkDistanceSquared(observer_chunk_coords, chunk_world_coords + offset), 
					neighbor_chunk->StopSource().get_token()
				}
			);
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

		const ChunkJob chunk_job = chunk_job_opt.value();
		assert(chunk_job->chunk_ != nullptr);
		const std::shared_ptr<Chunk> chunk = chunk_job.chunk_;

		if (chunk->StopSource().stop_requested() || chunk->GetMeshState() != MeshState::Invalid)
		{
			continue;
		}

		chunk->SetMeshState(MeshState::Building);
		
		thread_pool_.Enqueue(
			[this, 
			chunk, 
			chunk_mesh_dependencies = GetMeshDependencies(chunk->WorldCoords()), 
			&chunk_event_queue]()
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
			}
		);

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
	return glm::floor(position + pos_offset);
}

glm::ivec3 ChunkManager::RelativeBlockPos(glm::dvec3 position, glm::dvec3 pos_offset) const noexcept
{
	glm::ivec3 block_pos(0);

	const glm::dvec3 offset_pos = position + pos_offset;

	if (offset_pos.x < 0.0)
	{
		const int chunk_x_offset = (std::abs(static_cast<int>(offset_pos.x)) / constants::chunk::width) + 1;
		block_pos.x = (static_cast<int>(offset_pos.x) + chunk_x_offset * constants::chunk::width) - 1;
	}
	else
	{
		block_pos.x = static_cast<int>(offset_pos.x) % constants::chunk::width;
	}

	block_pos.y = static_cast<int>(std::floor(offset_pos.y));

	if (offset_pos.z < 0.0)
	{
		const int chunk_z_offset = (std::abs(static_cast<int>(offset_pos.z)) / constants::chunk::depth) + 1;
		block_pos.z = (static_cast<int>(offset_pos.z) + chunk_z_offset * constants::chunk::depth) - 1;
	}
	else
	{
		block_pos.z = static_cast<int>(offset_pos.z) % constants::chunk::depth;
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