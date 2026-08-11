#ifndef CHUNK_MANAGER_HPP
#define CHUNK_MANAGER_HPP

#include "utils/Hash.hpp"

#include "graphics/Camera.hpp"

#include "threading/ThreadSafeQueue.hpp"

#include "world/Chunk.hpp"
#include "world/ChunkEvents.hpp"
#include "world/Block.hpp"

#include <glm/vec2.hpp>

#include <mutex>
#include <shared_mutex>
#include <stop_token>
#include <unordered_map>
#include <queue>

class ChunkManager
{
private:
	ThreadPool& thread_pool_;
	std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, utils::ivec2_hash> chunks_;
	ThreadSafeQueue<Chunk*> chunk_build_queue_;
	ChunkID next_chunk_id_ = 1;
	std::mutex chunk_build_queue_mutex_;
	mutable std::shared_mutex chunks_shared_mutex_;

public:
	ChunkManager(ThreadPool& thread_pool);
	
    void InitChunks(int chunk_radius);
    
	void Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue, const Camera& camera);

	void MarkChunksForUnload(const Camera& camera);

	void LoadChunks(const Camera& camera);
	
	void UnloadChunks(ThreadSafeQueue<ChunkEvent>& chunk_event_queue);

	void BuildChunkMeshes(ThreadSafeQueue<ChunkEvent>& chunk_event_queue);

	std::unique_ptr<Mesh> BuildChunkMesh(Chunk& chunk, std::stop_token stop_token);

	BlockInfo WorldBlockQuery(glm::ivec2 current_chunk_coord, glm::ivec3 block_coords) const;

	const Chunk* GetChunkAt(glm::ivec2 chunk_coord) const;

	glm::ivec3 AbsoluteBlockPos(glm::dvec3 position, glm::dvec3 pos_offset = { 0.0, 0.0, 0.0 }) const noexcept;

	glm::ivec3 RelativeBlockPos(glm::dvec3 position, glm::dvec3 pos_offset = { 0.0, 0.0, 0.0 }) const noexcept;

	glm::ivec2 GetChunkCoords(glm::dvec3 pos) const noexcept;

	// TODO: TEMPORARY CHUNK FILL - REMOVE LATER!
	void FillChunkTmp(Chunk& chunk);

	// Getters
	const std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, utils::ivec2_hash>& Chunks() const { return chunks_; }

};

#endif // CHUNK_MANAGER_HPP

