#include "world/Chunk.hpp"

#include "core/Direction.hpp"

#include "render/GpuMesh3D.hpp"

#include "threading/ThreadSafeDeque.hpp"

#include "utils/Logger.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cassert>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <stop_token>

namespace
{
	constexpr std::array<glm::ivec3, 6> neighbor_offsets_ = {
		glm::ivec3{ 1, 0, 0 }, glm::ivec3{ -1, 0, 0 },
		glm::ivec3{ 0, 1, 0 }, glm::ivec3{ 0, -1, 0 },
		glm::ivec3{ 0, 0, 1 }, glm::ivec3{ 0, 0, -1 }
	};
}

Chunk::Chunk(ChunkID id, glm::ivec2 world_coords) : 
	id_(id), 
	mesh_id_(0), 
	world_coords_(world_coords), 
	pending_mesh_build_(std::nullopt), 
	terrain_state_(TerrainState::Invalid), 
	mesh_state_(MeshState::Invalid), 
	chunk_state_(ChunkState::Unloaded), 
	terrain_generated_(false)
{
}

Block& Chunk::BlockAt(glm::ivec3 coords, bool check_index)
{
	return const_cast<Block&>(std::as_const(*this).BlockAt(coords, check_index));
}

const Block& Chunk::BlockAt(glm::ivec3 coords, bool check_index) const
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

bool Chunk::IsValidIndex(glm::ivec3 coords) const
{
	const bool x_range_valid = coords.x >= 0 && coords.x < constants::chunk::width;
	const bool y_range_valid = coords.y >= 0 && coords.y < constants::chunk::height;
	const bool z_range_valid = coords.z >= 0 && coords.z < constants::chunk::depth;

	return x_range_valid && y_range_valid && z_range_valid;
}

Block Chunk::NeighborAt(glm::ivec3 coords, Direction dir) const
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

Block& Chunk::NeighborRefAt(glm::ivec3 coords, Direction dir)
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

std::uint64_t Chunk::IncrementMeshId() noexcept
{
	return ++mesh_id_;
}

bool Chunk::IsReadyToBuildMesh(const ChunkMeshDependencies& deps) const
{
    const Chunk* west_chunk = deps.GetChunk({ -1,  0 });
    const Chunk* east_chunk = deps.GetChunk({ 1,  0 });
    const Chunk* north_chunk = deps.GetChunk({ 0, -1 });
    const Chunk* south_chunk = deps.GetChunk({ 0,  1 });

    return terrain_state_ == TerrainState::Ready &&
        (west_chunk == nullptr || west_chunk->GetTerrainState() == TerrainState::Ready) &&
        (east_chunk == nullptr || east_chunk->GetTerrainState() == TerrainState::Ready) &&
        (north_chunk == nullptr || north_chunk->GetTerrainState() == TerrainState::Ready) &&
        (south_chunk == nullptr || south_chunk->GetTerrainState() == TerrainState::Ready);
}

std::optional<ChunkMeshBuildData> Chunk::TakePendingMeshBuild()
{
	std::lock_guard<std::mutex> lock(pending_mesh_build_mutex_);

	const auto cpy = pending_mesh_build_;

	ClearPendingMeshBuild();

	return cpy;
}

std::stop_token Chunk::GetMeshStopToken()
{
	std::lock_guard<std::mutex> lock(mesh_stop_source_mutex_);
	return mesh_building_stop_source_.get_token();
}

void Chunk::ResetMeshStopToken()
{
	std::lock_guard<std::mutex> lock(mesh_stop_source_mutex_);

	mesh_building_stop_source_.request_stop();
	mesh_building_stop_source_ = std::stop_source{};
}

bool Chunk::TrySetTerrainReady() noexcept
{
	TerrainState expected = TerrainState::Building;

	return terrain_state_.compare_exchange_strong(expected, TerrainState::Ready);
}

bool Chunk::TrySetMeshReady() noexcept
{
	MeshState expected = MeshState::Building;
	
	return mesh_state_.compare_exchange_strong(expected, MeshState::Ready);
}

int Chunk::Index(glm::ivec3 coords) const
{
	return coords.x + constants::chunk::width * (coords.z + constants::chunk::depth * coords.y);
}