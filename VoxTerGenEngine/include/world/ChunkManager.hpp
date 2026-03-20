#ifndef CHUNK_MANAGER_HPP
#define CHUNK_MANAGER_HPP

#include "world/Chunk.hpp"
#include "world/Block.hpp"

#include <glm/vec2.hpp>

#include <unordered_map>

struct ivec2_hash 
{
	std::size_t operator()(const glm::ivec2& vec) const noexcept;
};

class ChunkManager
{
private:
	std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, ivec2_hash> chunks_;

public:
	ChunkManager();

    void InitChunks(int chunk_radius);
    
    void InitChunkBlocks(Chunk& chunk);

	void BuildAllChunkMeshes();

	Block WorldBlockQuery(glm::ivec2 current_chunk_coord, const glm::ivec3& block_coords) const;

	const Chunk* GetChunkAt(glm::ivec2 chunk_coord) const;

	// Getters
	const std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, ivec2_hash>& Chunks() { return chunks_; }

};

#endif // CHUNK_MANAGER_HPP

