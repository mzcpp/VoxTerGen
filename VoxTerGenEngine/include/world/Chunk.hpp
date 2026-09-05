#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "core/Direction.hpp"

#include "mesh/Mesh3D.hpp"

#include "render/GpuMesh3D.hpp"

#include "utils/Constants.hpp"
#include "utils/Hash.hpp"

#include "world/Block.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <array>
#include <atomic>
#include <cstdint>
#include <memory>
#include <stop_token>
#include <unordered_map>
#include <variant>
#include <optional>

enum class TerrainState
{
    Invalid, 
    Building, 
    Ready
};

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

struct ChunkMesh
{
    Mesh3D cpu_opaque_mesh_;
    Mesh3D cpu_transparent_mesh_;
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

struct ChunkMeshBuildData
{
    std::uint64_t mesh_id_;
    double distance_squared_;
};

using ChunkID = std::uint64_t;

class Chunk
{
private:
    ChunkID id_;
    std::atomic<std::uint64_t> mesh_id_;
	glm::ivec2 world_coords_;
	std::array<Block, constants::chunk::size> blocks_;
    std::optional<ChunkMeshBuildData> pending_mesh_build_;
    std::atomic<TerrainState> terrain_state_;
    std::atomic<MeshState> mesh_state_;
    std::atomic<ChunkState> chunk_state_;
    std::stop_source mesh_building_stop_source_;
    std::atomic<bool> terrain_generated_;
    
public:
	explicit Chunk(ChunkID id, glm::ivec2 world_coords);

    Block& BlockAt(glm::ivec3 coords, bool check_index = false);

    const Block& BlockAt(glm::ivec3 coords, bool check_index = false) const;

    glm::ivec3 Pos(int index) const;

    bool IsValidIndex(glm::ivec3 coords) const;

    Block NeighborAt(glm::ivec3 coords, Direction dir) const;

    Block& NeighborRefAt(glm::ivec3 coords, Direction dir);

    std::uint64_t IncrementMeshId() noexcept;

    bool IsReadyToBuildMesh(const ChunkMeshDependencies& chunk_mesh_dependencies) const;

    // Getters
    ChunkID Id() const noexcept { return id_; }
    std::uint64_t MeshId() const noexcept { return mesh_id_; }
    glm::ivec2 WorldCoords() const noexcept { return world_coords_; }
    const std::array<Block, constants::chunk::size>& Blocks() const noexcept { return blocks_; }
    const std::optional<ChunkMeshBuildData>& GetPendingMeshBuild() const noexcept { return pending_mesh_build_; }
    TerrainState GetTerrainState() const noexcept { return terrain_state_; }
    MeshState GetMeshState() const noexcept { return mesh_state_; }
    ChunkState GetChunkState() const noexcept { return chunk_state_; }
    std::stop_source& StopSource() noexcept { return mesh_building_stop_source_; }
    const std::stop_source& StopSource() const noexcept { return mesh_building_stop_source_; }
    bool TerrainGenerated() const noexcept { return terrain_generated_; }

    // Setters
    void SetTerrainState(TerrainState terrain_state) noexcept { terrain_state_ = terrain_state; }
    void SetMeshState(MeshState mesh_state) noexcept { mesh_state_ = mesh_state; }
    void SetChunkState(ChunkState chunk_state) noexcept { chunk_state_ = chunk_state; }
    void SetPendingMeshBuild(ChunkMeshBuildData job_data) noexcept { pending_mesh_build_ = std::move(job_data); }
    void ClearPendingMeshBuild() noexcept { pending_mesh_build_ = std::nullopt; }
    void SetTerrainGenerated(bool terrain_generated) noexcept { terrain_generated_ = terrain_generated; }

private:
    int Index(glm::ivec3 coords) const;
};

#endif // CHUNK_HPP
