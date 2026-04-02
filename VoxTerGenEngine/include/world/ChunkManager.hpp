#ifndef CHUNK_MANAGER_HPP
#define CHUNK_MANAGER_HPP

#include "world/Chunk.hpp"
#include "world/Block.hpp"
#include "utils/Hash.hpp"
#include "graphics/Camera.hpp"

#include <glm/vec2.hpp>

#include <queue>
#include <unordered_map>
#include <queue>
#include <cstdint>

class ChunkManager
{
private:
	std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, utils::ivec2_hash> chunks_;
	std::queue<Chunk*> chunk_build_queue_;
	ChunkID next_chunk_id_ = 1;

public:
	ChunkManager();

    void InitChunks(int chunk_radius);
    
	void Tick(std::queue<ChunkEvent>& chunk_event_queue, const Camera& camera);

	void StreamChunks(std::queue<ChunkEvent>& chunk_event_queue, const Camera& camera);

	void BuildChunkMeshes(std::queue<ChunkEvent>& chunk_event_queue);

	std::unique_ptr<Mesh> BuildChunkMesh(Chunk& chunk);

	Block WorldBlockQuery(const glm::ivec2& current_chunk_coord, const glm::ivec3& block_coords) const;

	const Chunk* GetChunkAt(glm::ivec2 chunk_coord) const;

	void PushChunkIntoQueue(Chunk* chunk);

	glm::ivec2 GetChunkCoords(const glm::dvec3& pos) noexcept;

	// TODO: TEMPORARY CHUNK FILL - REMOVE LATER!
	void FillChunkTmp(Chunk& chunk);

	// Getters
	const std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, utils::ivec2_hash>& Chunks() const { return chunks_; }

};

#endif // CHUNK_MANAGER_HPP

