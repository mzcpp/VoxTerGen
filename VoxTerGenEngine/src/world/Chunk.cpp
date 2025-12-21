#include "world/Chunk.hpp"
#include "utils/Logger.hpp"

#include <glm/glm.hpp>

#include <stdexcept>
#include <memory>

Chunk::Chunk(glm::ivec2 world_coords) : world_coords_(world_coords)
{
	chunk_ = std::make_unique<std::array<Block, Constants::Chunk::size>>();
}

Block& Chunk::BlockAt(int x, int y, int z, bool check_index)
{
	return const_cast<Block&>(std::as_const(*this).BlockAt(x, y, z, check_index));
}

const Block& Chunk::BlockAt(int x, int y, int z, bool check_index) const
{
	if (check_index && !IsValidIndex(x, y, z))
	{
		Logger::Log(LogLevel::ERROR, "Index coordinate(s) out of range! x: {}, y: {}, z: {}", x, y, z);
		throw std::out_of_range("Index coordinate(s) out of range!");
	}

	return (*chunk_)[Index(x, y, z)];
}

glm::ivec3 Chunk::Pos(int index) const
{
	if (index < 0 || index >= Constants::Chunk::size)
	{
		Logger::Log(LogLevel::ERROR, "Index out of range! index: {}", index);
		throw std::out_of_range("Index out of range! index: " + std::to_string(index));
	}

	const int rem = index % (Constants::Chunk::width * Constants::Chunk::depth);
	const int z = rem / Constants::Chunk::width;
	const int x = rem % Constants::Chunk::width;
	const int y = index / (Constants::Chunk::width * Constants::Chunk::depth);

	return { x, y, z };
}

bool Chunk::IsValidIndex(int x, int y, int z) const
{
	const bool x_range_valid = x >= 0 && x < Constants::Chunk::width;
	const bool y_range_valid = y >= 0 && y < Constants::Chunk::height;
	const bool z_range_valid = z >= 0 && z < Constants::Chunk::depth;

	return x_range_valid && y_range_valid && z_range_valid;
}

Block Chunk::NeighborAt(int x, int y, int z, Direction dir) const
{
	const size_t dir_index = static_cast<size_t>(dir);
	
	if (dir_index >= neighbor_offsets_.size()) 
	{
		Logger::Log(LogLevel::ERROR, "Invalid direction value! Dir: {}", dir_index);
		assert(false);
		return Block(BlockType::Invalid);
	}

	const auto& offset = neighbor_offsets_[dir_index];
	return BlockAt(x + offset.x, y + offset.y, z + offset.z);
}

Block& Chunk::NeighborRefAt(int x, int y, int z, Direction dir)
{
	if (!IsValidIndex(x, y, z))
	{
		Logger::Log(LogLevel::ERROR, "Index coordinate(s) out of range! x: {}, y: {}, z: {}", x, y, z);
		throw std::out_of_range("Index coordinate(s) out of range!");
	}

	const size_t dir_index = static_cast<size_t>(dir);
	assert(dir_index < neighbor_offsets_.size()); // catch invalid enum

	const auto& offset = neighbor_offsets_[dir_index];
	return BlockAt(x + offset.x, y + offset.y, z + offset.z);
}

void Chunk::Fill(std::function<Block(int, int, int)> filler)
{
	for (int y = 0; y < Constants::Chunk::height; ++y)
	{
		for (int z = 0; z < Constants::Chunk::depth; ++z)
		{
			for (int x = 0; x < Constants::Chunk::width; ++x)
			{
				(*chunk_)[Index(x, y, z)] = filler(x, y, z);
			}
		}
	}
}

int Chunk::Index(int x, int y, int z) const
{
	return x + Constants::Chunk::width * (z + Constants::Chunk::depth * y);
}