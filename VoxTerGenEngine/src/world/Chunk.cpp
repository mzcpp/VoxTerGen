#include "world/Chunk.hpp"
#include "utils/Logger.hpp"
#include "core/Direction.hpp"
#include "render/GpuMesh.hpp"

#include <glm/glm.hpp>

#include <stdexcept>
#include <memory>

Chunk::Chunk(glm::ivec2 world_coords) :
	world_coords_(world_coords),
	mesh_(nullptr),
	gpu_mesh_(std::make_unique<class GpuMesh>()),
	mesh_valid_(false),
	mesh_needs_upload_(false)
{
}

Block& Chunk::BlockAt(const glm::ivec3& coords, bool check_index)
{
	return const_cast<Block&>(std::as_const(*this).BlockAt(coords, check_index));
}

const Block& Chunk::BlockAt(const glm::ivec3& coords, bool check_index) const
{
	if (check_index && !IsValidIndex(coords))
	{
		Logger::Log(LogLevel::ERROR, "Index coordinate(s) out of range! x: {}, y: {}, z: {}", coords.x, coords.y, coords.z);
		throw std::out_of_range("Index coordinate(s) out of range!");
	}

	return blocks_[Index(coords)];
}

glm::ivec3 Chunk::Pos(int index) const
{
	if (index < 0 || index >= constants::chunk::size)
	{
		Logger::Log(LogLevel::ERROR, "Index out of range! index: {}", index);
		throw std::out_of_range("Index out of range! index: " + std::to_string(index));
	}

	const int rem = index % (constants::chunk::width * constants::chunk::depth);
	const int z = rem / constants::chunk::width;
	const int x = rem % constants::chunk::width;
	const int y = index / (constants::chunk::width * constants::chunk::depth);

	return { x, y, z };
}

bool Chunk::IsValidIndex(const glm::ivec3& coords) const
{
	const bool x_range_valid = coords.x >= 0 && coords.x < constants::chunk::width;
	const bool y_range_valid = coords.y >= 0 && coords.y < constants::chunk::height;
	const bool z_range_valid = coords.z >= 0 && coords.z < constants::chunk::depth;

	return x_range_valid && y_range_valid && z_range_valid;
}

Block Chunk::NeighborAt(const glm::ivec3& coords, Direction dir) const
{
	const std::size_t dir_index = static_cast<std::size_t>(dir);
	
	if (dir_index >= neighbor_offsets_.size()) 
	{
		Logger::Log(LogLevel::ERROR, "Invalid direction value! Dir: {}", dir_index);
		assert(false);
		return Block(BlockType::Invalid);
	}

	const auto& offset = neighbor_offsets_[dir_index];
	return BlockAt({ coords.x + offset.x, coords.y + offset.y, coords.z + offset.z } );
}

Block& Chunk::NeighborRefAt(const glm::ivec3& coords, Direction dir)
{
	if (!IsValidIndex(coords))
	{
		Logger::Log(LogLevel::ERROR, "Index coordinate(s) out of range! x: {}, y: {}, z: {}", coords.x, coords.y, coords.z);
		throw std::out_of_range("Index coordinate(s) out of range!");
	}

	const std::size_t dir_index = static_cast<std::size_t>(dir);
	assert(dir_index < neighbor_offsets_.size()); // catch invalid enum

	const auto& offset = neighbor_offsets_[dir_index];
	return BlockAt({ coords.x + offset.x, coords.y + offset.y, coords.z + offset.z });
}

void Chunk::ReleaseMeshData()
{
	mesh_.reset(nullptr);
}

int Chunk::Index(const glm::ivec3& coords) const
{
	return coords.x + constants::chunk::width * (coords.z + constants::chunk::depth * coords.y);
}