#ifndef CHUNK_MANAGER_HPP
#define CHUNK_MANAGER_HPP

#include "world/Chunk.hpp"
#include "world/Block.hpp"
#include "utils/Hash.hpp"

#include <glm/vec2.hpp>

#include <unordered_map>
#include <queue>

class ChunkManager
{
private:
	std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, utils::ivec2_hash> chunks_;
	std::queue<Chunk*> chunk_build_queue_;

public:
	ChunkManager();

    void InitChunks(int chunk_radius);
    
    void InitChunkBlocks(Chunk& chunk);

	void Tick();

	void BuildChunkMeshes();

	void BuildChunkMesh(Chunk& chunk);

	Block WorldBlockQuery(const glm::ivec2& current_chunk_coord, const glm::ivec3& block_coords) const;

	const Chunk* GetChunkAt(glm::ivec2 chunk_coord) const;

	Chunk* GetChunkAt(glm::ivec2 chunk_coord);

	void PushChunkIntoQueue(Chunk* chunk);

	// Getters
	const std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, utils::ivec2_hash>& Chunks() const { return chunks_; }

};

#endif // CHUNK_MANAGER_HPP

