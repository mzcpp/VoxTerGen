#ifndef CHUNK_MANAGER_HPP
#define CHUNK_MANAGER_HPP

#include "world/Chunk.hpp"
#include "world/Block.hpp"

#include <glm/vec2.hpp>

#include <unordered_map>

struct ivec2_hash 
{
    std::size_t operator()(const glm::ivec2& v) const noexcept 
    {
        const std::size_t h1 = std::hash<int>{}(v.x);
        const std::size_t h2 = std::hash<int>{}(v.y);
        return h1 ^ (h2 << 1);
    }
};

class ChunkManager
{
private:
	std::unordered_map<glm::ivec2, Chunk, ivec2_hash> chunks_;

public:
	ChunkManager();

	//Block& NeighborAt(glm::ivec2 chunk_coord, int x, int y, int z, Direction dir) const;

	std::unordered_map<glm::ivec2, Chunk, ivec2_hash>& Chunks() { return chunks_; }

	//Chunk& ChunkAt(glm::ivec2 chunk_coord);
};

#endif // CHUNK_MANAGER_HPP

