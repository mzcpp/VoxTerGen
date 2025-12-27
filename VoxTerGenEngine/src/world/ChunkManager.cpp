#include "world/ChunkManager.hpp"

ChunkManager::ChunkManager()
{
	InitChunks(Constants::Chunk::default_radius);
}

//Block& ChunkManager::NeighborAt(glm::ivec2 chunk_coord, int x, int y, int z, Direction dir) const
//{
//}