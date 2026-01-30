#include "world/ChunkManager.hpp"
#include "world/Chunk.hpp"
#include "core/Settings.hpp"
#include "utils/Logger.hpp"

#include <cassert>

std::size_t ivec2_hash::operator()(const glm::ivec2& vec) const noexcept
{
	const std::size_t h1 = std::hash<int>{}(vec.x);
	const std::size_t h2 = std::hash<int>{}(vec.y);
	return h1 ^ (h2 << 1);
}

ChunkManager::ChunkManager()
{
	InitChunks(Constants::Chunk::default_radius);
}

void ChunkManager::InitChunks(int chunk_radius)
{
	const int chunk_square_size = 2 * chunk_radius + 1;
	const glm::ivec3 start_coords = { 0 - chunk_radius, 0, 0 - chunk_radius };

	for (int z = 0; z < chunk_square_size; ++z)
	{
		for (int x = 0; x < chunk_square_size; ++x)
		{
			const glm::ivec2 world_coords = { start_coords.x + x, start_coords.z + z };
			std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(world_coords);
			InitChunkBlocks(*chunk);
			chunks_.emplace(world_coords, std::move(chunk));
		}
	}

	// TEST CASES, CHUNK RADIUS 1 REQUIRED, 3x3x3 chunks

	// neighbors of first block of first chunk
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 0, 0, 0, Direction::PosX).IsSolid()); // air
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 0, 0, 0, Direction::PosY).IsSolid()); // air
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 0, 0, 0, Direction::PosZ).IsSolid()); // air

	// setting them to stone
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(1, 0, 0).SetType(BlockType::Stone);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(0, 1, 0).SetType(BlockType::Stone);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(0, 0, 1).SetType(BlockType::Stone);

	// checking again
	assert(NeighborAt({ start_coords.x, start_coords.z }, 0, 0, 0, Direction::PosX).IsSolid());
	assert(NeighborAt({ start_coords.x, start_coords.z }, 0, 0, 0, Direction::PosY).IsSolid());
	assert(NeighborAt({ start_coords.x, start_coords.z }, 0, 0, 0, Direction::PosZ).IsSolid());

	assert(!NeighborAt({ start_coords.x, start_coords.z }, 0, 0, 0, Direction::NegX).IsSolid()); // no chunk there
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 0, 0, 0, Direction::NegY).IsSolid()); // no chunk there
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 0, 0, 0, Direction::NegZ).IsSolid()); // no chunk there


	chunks_.at({ start_coords.x + 1, start_coords.z })->BlockAt(0, 0, 0).SetType(BlockType::Stone);
	chunks_.at({ start_coords.x, start_coords.z + 1 })->BlockAt(0, 0, 0).SetType(BlockType::Stone);

	assert(NeighborAt({ start_coords.x, start_coords.z }, Constants::Chunk::width - 1, 0, 0, Direction::PosX).IsSolid()); // should be stone
	assert(!NeighborAt({ start_coords.x, start_coords.z }, Constants::Chunk::width - 1, 0, 0, Direction::NegY).IsSolid()); // no chunk
	assert(!NeighborAt({ start_coords.x, start_coords.z }, Constants::Chunk::width - 1, 0, 0, Direction::NegZ).IsSolid()); // no chunk

	assert(!NeighborAt({ start_coords.x, start_coords.z }, 0, Constants::Chunk::height - 1, 0, Direction::NegX).IsSolid()); // no chunk
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 0, Constants::Chunk::height - 1, 0, Direction::PosY).IsSolid()); // no chunk
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 0, Constants::Chunk::height - 1, 0, Direction::NegZ).IsSolid()); // no chunk

	assert(!NeighborAt({ start_coords.x, start_coords.z }, 0, 0, Constants::Chunk::depth - 1, Direction::NegX).IsSolid()); // no chunk
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 0, 0, Constants::Chunk::depth - 1, Direction::NegY).IsSolid()); // no chunk
	assert(NeighborAt({ start_coords.x, start_coords.z }, 0, 0, Constants::Chunk::depth - 1, Direction::PosZ).IsSolid()); // should be stone

	// checking neighbors of center block of the first chunks
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(2, 1, 1).SetType(BlockType::Stone);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(1, 2, 1).SetType(BlockType::Stone);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(1, 1, 2).SetType(BlockType::Stone);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(0, 1, 1).SetType(BlockType::Stone);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(1, 0, 1).SetType(BlockType::Stone);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(1, 1, 0).SetType(BlockType::Stone);
	assert(NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::PosX).IsSolid());
	assert(NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::PosY).IsSolid());
	assert(NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::PosZ).IsSolid());
	assert(NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::NegX).IsSolid());
	assert(NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::NegY).IsSolid());
	assert(NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::NegZ).IsSolid());
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(2, 1, 1).SetType(BlockType::Air);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(1, 2, 1).SetType(BlockType::Air);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(1, 1, 2).SetType(BlockType::Air);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(0, 1, 1).SetType(BlockType::Air);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(1, 0, 1).SetType(BlockType::Air);
	chunks_.at({ start_coords.x, start_coords.z })->BlockAt(1, 1, 0).SetType(BlockType::Air);
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::PosX).IsSolid());
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::PosY).IsSolid());
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::PosZ).IsSolid());
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::NegX).IsSolid());
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::NegY).IsSolid());
	assert(!NeighborAt({ start_coords.x, start_coords.z }, 1, 1, 1, Direction::NegZ).IsSolid());

	// CENTERS OF SIDES OF THE MIDDLE CHUNK
	// set to stone in neighboring chunks
	chunks_.at({ start_coords.x + 1, start_coords.z })->BlockAt(1, 1, 2).SetType(BlockType::Stone);
	chunks_.at({ start_coords.x, start_coords.z + 1 })->BlockAt(2, 1, 1).SetType(BlockType::Stone);
	chunks_.at({ start_coords.x + 1, start_coords.z + 2 })->BlockAt(1, 1, 0).SetType(BlockType::Stone);
	chunks_.at({ start_coords.x + 2, start_coords.z + 1 })->BlockAt(0, 1, 1).SetType(BlockType::Stone);

	// check
	assert(NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 1, 1, 0, Direction::NegZ).IsSolid());
	assert(NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 0, 1, 1, Direction::NegX).IsSolid());
	assert(!NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 1, 0, 1, Direction::NegY).IsSolid()); // no chunk

	assert(NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 2, 1, 1, Direction::PosX).IsSolid());
	assert(!NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 1, 2, 1, Direction::PosY).IsSolid()); // no chunk
	assert(NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 1, 1, 2, Direction::PosZ).IsSolid());

	// set back to air in neighboring chunks
	chunks_.at({ start_coords.x + 1, start_coords.z })->BlockAt(1, 1, 2).SetType(BlockType::Air);
	chunks_.at({ start_coords.x, start_coords.z + 1 })->BlockAt(2, 1, 1).SetType(BlockType::Air);
	chunks_.at({ start_coords.x + 1, start_coords.z + 2 })->BlockAt(1, 1, 0).SetType(BlockType::Air);
	chunks_.at({ start_coords.x + 2, start_coords.z + 1 })->BlockAt(0, 1, 1).SetType(BlockType::Air);
	
	// check again
	assert(!NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 1, 1, 0, Direction::NegZ).IsSolid());
	assert(!NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 0, 1, 1, Direction::NegX).IsSolid());
	assert(!NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 1, 0, 1, Direction::NegY).IsSolid()); // no chunk

	assert(!NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 2, 1, 1, Direction::PosX).IsSolid());
	assert(!NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 1, 2, 1, Direction::PosY).IsSolid()); // no chunk
	assert(!NeighborAt({ start_coords.x + 1, start_coords.z + 1 }, 1, 1, 2, Direction::PosZ).IsSolid());
}

void ChunkManager::InitChunkBlocks(Chunk& chunk)
{
	for (int y = 0; y < Constants::Chunk::height; ++y)
	{
		for (int z = 0; z < Constants::Chunk::depth; ++z)
		{
			for (int x = 0; x < Constants::Chunk::width; ++x)
			{
				chunk.BlockAt(x, y, z).SetType(BlockType::Air);
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