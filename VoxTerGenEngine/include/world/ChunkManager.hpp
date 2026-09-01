#ifndef CHUNK_MANAGER_HPP
#define CHUNK_MANAGER_HPP

#include "graphics/Camera.hpp"

#include "render/events/ChunkEvents.hpp"

#include "threading/ThreadSafeQueue.hpp"
#include "threading/ThreadSafePriorityQueue.hpp"

#include "utils/Hash.hpp"

#include "world/Block.hpp"
#include "world/Chunk.hpp"
#include "world/Observer.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <memory>
#include <mutex>
#include <shared_mutex>
#include <stop_token>
#include <unordered_map>
#include <vector>
#include <queue>

class ThreadPool;

struct ChunkJob
{
	std::shared_ptr<Chunk> chunk_;
	std::uint64_t mesh_id_;
	double distance_squared_;
	std::stop_token stop_token_;
};

struct ChunkJobCompare
{
	bool operator()(const ChunkJob& a, const ChunkJob& b) const
	{
		return a.distance_squared_ > b.distance_squared_;
	}
};

class ChunkManager
{
private:
	Observer& observer_;
	ThreadPool& thread_pool_;
	std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>, utils::ivec2_hash> chunks_;
	ThreadSafePriorityQueue<ChunkJob, ChunkJobCompare> chunk_build_queue_;
	ChunkID next_chunk_id_ = 1;
	mutable std::shared_mutex chunks_shared_mutex_;
	std::vector<glm::ivec2> chunks_to_load_;
	std::vector<glm::ivec2> chunks_to_unload_;

public:
	ChunkManager(Observer& observer, ThreadPool& thread_pool);
	
    void InitChunks(int chunk_radius);
    
	void Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue);

	void MarkChunksForUnload();

	void FindChunksToLoad();

	void UnloadChunks(ThreadSafeQueue<ChunkEvent>& chunk_event_queue);
	
	void LoadChunks();

	void EnqueueChunkMeshBuild(Chunk& chunk, double distance);

	void ScheduleChunkMeshBuilds();

	void ScheduleNeighborChunkMeshBuilds(glm::ivec2 observer_chunk_coords, glm::ivec2 chunk_world_coords);

	void BuildChunkMeshes(ThreadSafeQueue<ChunkEvent>& chunk_event_queue);

	std::unique_ptr<ChunkMesh> BuildChunkMesh(const ChunkMeshDependencies& chunk_mesh_dependencies, std::stop_token stop_token);

	BlockInfo WorldBlockQuery(glm::ivec2 current_chunk_coord, glm::ivec3 block_coords) const;

	std::shared_ptr<Chunk> GetChunkAt(glm::ivec2 chunk_coord) const;

	glm::ivec3 AbsoluteBlockPos(glm::dvec3 position, glm::dvec3 pos_offset = { 0.0, 0.0, 0.0 }) const noexcept;

	glm::ivec3 RelativeBlockPos(glm::dvec3 position, glm::dvec3 pos_offset = { 0.0, 0.0, 0.0 }) const noexcept;

	glm::ivec2 GetChunkCoords(glm::dvec3 pos) const noexcept;

	double ChunkDistanceSquared(glm::ivec2 first, glm::ivec2 second) const noexcept;

	ChunkMeshDependencies GetMeshDependencies(glm::ivec2 coords) const;

	// TODO: TEMPORARY CHUNK FILL - REMOVE LATER!
	void FillChunkTmp(Chunk& chunk);

	// Getters
	const std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>, utils::ivec2_hash>& Chunks() const { return chunks_; }

};

#endif // CHUNK_MANAGER_HPP

