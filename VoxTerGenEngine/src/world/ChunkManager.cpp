#include "world/ChunkManager.hpp"
#include "world/Chunk.hpp"
#include "core/Settings.hpp"

std::size_t ivec2_hash::operator()(const glm::ivec2& vec) const noexcept
{
	const std::size_t h1 = std::hash<int>{}(vec.x);
	const std::size_t h2 = std::hash<int>{}(vec.y);
	return h1 ^ (h2 << 1);
}

ChunkManager::ChunkManager()
{
}

void ChunkManager::InitChunks(int chunk_radius)
{
	const int chunk_square_size = 2 * chunk_radius + 1;

	const glm::ivec3 start_coords = { 0 - chunk_radius, 0, 0 - chunk_radius };

	for (int chunk_z = start_coords.z; chunk_z < chunk_square_size; ++chunk_z)
	{
		for (int chunk_x = start_coords.x; chunk_x < chunk_square_size; ++chunk_x)
		{
			const glm::ivec2 world_coords = { chunk_x, chunk_z };
			Chunk chunk(world_coords);
			InitChunkBlocks(chunk);
			chunks_.emplace(world_coords, chunk);
		}
	}
}

void ChunkManager::InitChunkBlocks(Chunk& chunk)
{
	for (int y = 0; y < Constants::Chunk::height; ++y)
	{
		for (int z = 0; z < Constants::Chunk::depth; ++z)
		{
			for (int x = 0; x < Constants::Chunk::width; ++x)
			{
				chunk.BlockAt(x, y, z).SetType(BlockType::Stone);
			}
		}
	}
}


//Block& ChunkManager::NeighborAt(glm::ivec2 chunk_coord, int x, int y, int z, Direction dir) const
//{
//}