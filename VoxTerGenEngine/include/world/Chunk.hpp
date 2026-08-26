#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "core/Direction.hpp"

#include "render/GpuMesh.hpp"

#include "utils/Constants.hpp"
#include "utils/Hash.hpp"

#include "world/Block.hpp"

#include "mesh/Mesh.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <array>
#include <memory>
#include <variant>
#include <cstdint>
#include <unordered_map>
#include <stop_token>
#include <atomic>

enum class MeshState
{
    Invalid, 
    Building, 
    Cancelled, 
    Ready
};

enum class ChunkState
{
    Unloaded, 
    PendingUnload, 
    Loaded
};

class Chunk;

struct ChunkMeshDependencies
{
    // 3x3 neighborhood:
    // [0] [1] [2]
    // [3] [4] [5]
    // [6] [7] [8]
    std::array<std::shared_ptr<Chunk>, 9> chunks_;

    Chunk* GetChunk(glm::ivec2 offset) const noexcept
    {
        return chunks_[(offset.y + 1) * 3 + (offset.x + 1)].get();
    }
};

struct ChunkMesh
{
    Mesh cpu_opaque_mesh_;
    Mesh cpu_transparent_mesh_;
};

using ChunkID = std::uint64_t;

class Chunk
{
private:
    ChunkID id_;
	glm::ivec2 world_coords_;
	std::array<Block, constants::chunk::size> blocks_;
    std::atomic<MeshState> mesh_state_;
    std::atomic<ChunkState> chunk_state_;
    std::stop_source mesh_building_stop_source_;
    
public:
	explicit Chunk(ChunkID id, glm::ivec2 world_coords);

    Block& BlockAt(glm::ivec3 coords, bool check_index = false);

    const Block& BlockAt(glm::ivec3 coords, bool check_index = false) const;

    glm::ivec3 Pos(int index) const;

    bool IsValidIndex(glm::ivec3 coords) const;

    Block NeighborAt(glm::ivec3 coords, Direction dir) const;

    Block& NeighborRefAt(glm::ivec3 coords, Direction dir);

    // Getters
    ChunkID Id() const noexcept { return id_; }
    glm::ivec2 WorldCoords() const noexcept { return world_coords_; }
    const std::array<Block, constants::chunk::size>& Blocks() const noexcept { return blocks_; }
    MeshState GetMeshState() const noexcept { return mesh_state_; }
    ChunkState GetChunkState() const noexcept { return chunk_state_; }
    std::stop_source& StopSource() noexcept { return mesh_building_stop_source_; }
    const std::stop_source& StopSource() const noexcept { return mesh_building_stop_source_; }

    // Setters
    void SetMeshState(MeshState mesh_state) { mesh_state_ = mesh_state; }
    void SetChunkState(ChunkState chunk_state) { chunk_state_ = chunk_state; }

private:
    int Index(glm::ivec3 coords) const;
};

#endif // CHUNK_HPP
