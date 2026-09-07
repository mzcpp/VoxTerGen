#include "world/ChunkManager.hpp"

#include "generator/TerrainGenerator.hpp"

#include "mesh/MeshBuilder.hpp"

#include "render/events/ChunkEvents.hpp"

#include "threading/ThreadPool.hpp"
#include "threading/ThreadSafeQueue.hpp"

#include "utils/MathUtils.hpp"
#include "utils/Constants.hpp"

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
	thread_pool_(thread_pool), 
	terrain_generator_(NoiseType::PERLIN, 1426728)
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

void ChunkManager::Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	MarkChunksForUnload();
	FindChunksToLoad();
	
	UnloadChunks(chunk_event_queue);
	LoadChunks();

	ScheduleChunkTerrainBuild();

	BuildChunkTerrains();
	BuildChunkMeshes(chunk_event_queue);
}

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
			chunk->ResetMeshStopToken();
			chunk->SetChunkState(ChunkState::PendingUnload);
			chunk->SetMeshState(MeshState::Cancelled);
			chunk->SetTerrainState(TerrainState::Cancelled);

			chunks_to_unload_.push_back(chunk_world_coords);
		}
	}
}

void ChunkManager::FindChunksToLoad()
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
			chunk.SetTerrainGenerated(false);

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
	std::lock_guard<std::shared_mutex> lock(chunks_shared_mutex_);

	for (glm::ivec2 chunk_coords : chunks_to_load_)
	{
		if (chunks_.find(chunk_coords) != chunks_.end())
		{
			continue;
		}

		std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(next_chunk_id_++, chunk_coords);

		chunk->SetChunkState(ChunkState::Loaded);
		chunk->SetMeshState(MeshState::Invalid);

		chunks_.try_emplace(chunk_coords, std::move(chunk));
	}
}

void ChunkManager::EnqueueChunkMeshBuild(const std::shared_ptr<Chunk>& chunk, double distance)
{
	assert(chunk != nullptr);

	chunk->ResetMeshStopToken();
	chunk->SetMeshState(MeshState::Invalid);
	chunk->IncrementMeshId();

	std::lock_guard lock(chunk->PendingMeshBuildMutex());

	if (!chunk->GetPendingMeshBuild().has_value())
	{
		chunk_mesh_build_deque_.PushBack(chunk);
	}

	chunk->SetPendingMeshBuild(ChunkMeshBuildData{ chunk->MeshId(), distance });
}

void ChunkManager::ScheduleChunkTerrainBuild()
{
	std::shared_lock<std::shared_mutex> lock(chunks_shared_mutex_);

	for (glm::ivec2 chunk_world_coords : chunks_to_load_)
	{
		std::shared_ptr<Chunk> current_chunk = GetChunkAt(chunk_world_coords);

		if (current_chunk == nullptr)
		{
			continue;
		}

		if (!current_chunk->TerrainGenerated())
		{
			chunk_terrain_build_deque_.push_front(current_chunk);
		}
	}
}

void ChunkManager::ScheduleNeighborChunkMeshBuilds(glm::ivec2 observer_chunk_coords, glm::ivec2 chunk_world_coords)
{
	std::shared_lock<std::shared_mutex> lock(chunks_shared_mutex_);

	for (int j = 0; j < 2; ++j)
	{
		for (int i : { -1, 1 })
		{
			glm::ivec2 offset = { i, 0 };

			if (j == 1)
			{
				std::swap(offset.x, offset.y);
			}

			const std::shared_ptr<Chunk> neighbor_chunk = GetChunkAt(chunk_world_coords + offset);

			if (neighbor_chunk == nullptr)
			{
				continue;
			}

			const ChunkMeshDependencies dependencies = GetMeshDependenciesUnlocked(neighbor_chunk->WorldCoords());

			if (!neighbor_chunk->IsReadyToBuildMesh(dependencies))
			{
				continue;
			}

			EnqueueChunkMeshBuild(neighbor_chunk, ChunkDistanceSquared(observer_chunk_coords, neighbor_chunk->WorldCoords()));
		}
	}
}

void ChunkManager::BuildChunkTerrains()
{
	int jobs_submitted = 0;
	const glm::ivec2 observer_chunk_coords = GetChunkCoords(observer_.Pos());
	std::unordered_map<glm::ivec2, double, utils::ivec2_hash> distances;

	for (const std::shared_ptr<Chunk>& chunk : chunk_terrain_build_deque_)
	{
		distances.try_emplace(chunk->WorldCoords(), ChunkDistanceSquared(observer_chunk_coords, chunk->WorldCoords()));
	}

	std::ranges::sort(chunk_terrain_build_deque_, 
		[&distances](const std::shared_ptr<Chunk>& left, const std::shared_ptr<Chunk>& right)
		{
			const auto left_it = distances.find(left->WorldCoords());
			const auto right_it = distances.find(right->WorldCoords());

			assert(left_it != distances.end());
			assert(right_it != distances.end());

			return left_it->second < right_it->second;
		}
	);

	while (jobs_submitted < constants::threading::jobs_submitted_limit && !chunk_terrain_build_deque_.empty())
	{
		const std::shared_ptr<Chunk> chunk = chunk_terrain_build_deque_.front();

		//if (chunk->WorldCoords().x == 0 && chunk->WorldCoords().y == 0)
		//{
		//	int x = 1;
		//}

		chunk_terrain_build_deque_.pop_front();

		assert(chunk != nullptr);
		assert(!chunk->TerrainGenerated());

		if (chunk->GetMeshStopToken().stop_requested() || chunk->GetMeshState() != MeshState::Invalid)
		{
			continue;
		}

		chunk->SetTerrainState(TerrainState::Building);

		thread_pool_.Enqueue(
			[this, 
			observer_chunk_coords, 
			chunk, 
			stop_token = chunk->GetMeshStopToken()]()
			{
				//FillChunkTmp(*chunk);
				terrain_generator_.GenerateChunkTerrain(chunk, stop_token);

				if (!chunk->TrySetTerrainReady())
				{
					return;
				}

				if (chunk->IsReadyToBuildMesh(GetMeshDependencies(chunk->WorldCoords())))
				{
					EnqueueChunkMeshBuild(chunk, ChunkDistanceSquared(observer_chunk_coords, chunk->WorldCoords()));
				}

				ScheduleNeighborChunkMeshBuilds(observer_chunk_coords, chunk->WorldCoords());
			}
		);

		++jobs_submitted;
	}
}

void ChunkManager::BuildChunkMeshes(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	const glm::ivec2 observer_chunk_coords = GetChunkCoords(observer_.Pos());

	for (glm::ivec2 chunk_world_coords : chunks_to_unload_)
	{
		ScheduleNeighborChunkMeshBuilds(observer_chunk_coords, chunk_world_coords);
	}

	int jobs_submitted = 0;

	while (const std::optional<std::shared_ptr<Chunk>> chunk_opt = chunk_mesh_build_deque_.TryPopFront())
	{
		const std::shared_ptr<Chunk>& chunk = chunk_opt.value();

		assert(chunk != nullptr);

		const auto chunk_build_data_opt = chunk->TakePendingMeshBuild();

		if (!chunk_build_data_opt.has_value() || chunk->GetMeshStopToken().stop_requested() || chunk->GetMeshState() != MeshState::Invalid)
		{
			continue;
		}

		chunk->SetMeshState(MeshState::Building);
		
		thread_pool_.Enqueue(
			[this, 
			chunk, 
			chunk_build_data = chunk_build_data_opt.value(),
			stop_token = chunk->GetMeshStopToken(),
			chunk_mesh_dependencies = GetMeshDependencies(chunk->WorldCoords()), 
			&chunk_event_queue]()
			{
				std::unique_ptr<ChunkMesh> chunk_mesh = BuildChunkMesh(chunk_mesh_dependencies, stop_token);

				if (chunk_mesh == nullptr || chunk_build_data.mesh_id_ != chunk->MeshId())
				{
					return;
				}

				chunk_event_queue.Push(ChunkMeshReady{ chunk, chunk_build_data.mesh_id_, std::move(chunk_mesh) });

				chunk->TrySetMeshReady();
			}
		);

		if (++jobs_submitted >= constants::threading::jobs_submitted_limit)
		{
			break;
		}
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
    std::shared_lock lock(chunks_shared_mutex_);

	return GetMeshDependenciesUnlocked(chunk_coords);
}

ChunkMeshDependencies ChunkManager::GetMeshDependenciesUnlocked(glm::ivec2 chunk_coords) const
{
	ChunkMeshDependencies chunk_mesh_dependencies;

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