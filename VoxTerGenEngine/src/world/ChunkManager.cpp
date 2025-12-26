#include "world/ChunkManager.hpp"
#include "world/Chunk.hpp"
#include "core/Settings.hpp"
#include "utils/Logger.hpp"

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
			std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(world_coords);
			InitChunkBlocks(*chunk);
			chunks_.emplace(world_coords, std::move(chunk));
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

const Chunk* ChunkManager::GetChunkAt(glm::ivec2 chunk_coord) const
{
	const auto it = chunks_.find(chunk_coord);

	if (it == chunks_.end())
	{
		return nullptr;
	}

	return it->second.get();
}

Block ChunkManager::NeighborAt(glm::ivec2 chunk_coord, int x, int y, int z, Direction dir) const
{
	if (dir == Direction::PosX && x == Constants::Chunk::width - 1)
	{
		const Chunk* right_chunk = GetChunkAt({ chunk_coord.x + 1, chunk_coord.y });

		if (!right_chunk)
		{
			return Block();
		}

		return right_chunk->BlockAt(0, y, z);
	}
	else if (dir == Direction::NegX && x == 0)
	{
		const Chunk* left_chunk = GetChunkAt({ chunk_coord.x - 1, chunk_coord.y });

		if (!left_chunk)
		{
			return Block();
		}

		return left_chunk->BlockAt(Constants::Chunk::width - 1, y, z);
	}
	else if (dir == Direction::PosY && y == Constants::Chunk::height - 1)
	{
		return Block();
	}
	else if (dir == Direction::NegY && y == 0)
	{
		return Block();
	}
	else if (dir == Direction::PosZ && z == Constants::Chunk::depth - 1)
	{
		const Chunk* front_chunk = GetChunkAt({ chunk_coord.x, chunk_coord.y + 1 });

		if (!front_chunk)
		{
			return Block();
		}

		return front_chunk->BlockAt(x, y, 0);
	}
	else if (dir == Direction::NegZ && z == 0)
	{
		const Chunk* back_chunk = GetChunkAt({ chunk_coord.x, chunk_coord.y - 1 });

		if (!back_chunk)
		{
			return Block();
		}

		return back_chunk->BlockAt(x, y, Constants::Chunk::depth - 1);
	}
	else
	{
		const Chunk* chunk = GetChunkAt({ chunk_coord.x, chunk_coord.y });

		if (!chunk)
		{
			Logger::Log(LogLevel::ERROR, "Chunk with coordinates {} {} is missing!", chunk_coord.x, chunk_coord.y);
			assert(false);
		}

		switch (dir)
		{
			case Direction::PosX:
				return chunk->BlockAt(x + 1, y, z);
			case Direction::NegX:
				return chunk->BlockAt(x - 1, y, z);
			case Direction::PosY:
				return chunk->BlockAt(x, y + 1, z);
			case Direction::NegY:
				return chunk->BlockAt(x, y - 1, z);
			case Direction::PosZ:
				return chunk->BlockAt(x, y, z + 1);
			case Direction::NegZ:
				return chunk->BlockAt(x, y, z - 1);
			default:
				Logger::Log(LogLevel::ERROR, "Unknown NeighborAt() direction!");
				assert(false);
		}
	}
}