#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "world/Block.hpp"
#include "utils/Constants.hpp"
#include "mesh/Mesh.hpp"
#include "render/GpuMesh.hpp"
#include "core/Direction.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <memory>
#include <variant>
#include <cstdint>

using ChunkID = std::uint64_t;

struct ChunkMeshReady
{
    ChunkID chunk_id_;
    std::ivec2 world_coords_;
    std::unique_ptr<Mesh> cpu_mesh_;
};

struct ChunkDestroyed
{
    ChunkID chunk_id_;
};

using ChunkEvent = std::variant<ChunkMeshReady, ChunkDestroyed>;

class Chunk
{
private:
    ChunkID id_;
	glm::ivec2 world_coords_;
	std::array<Block, constants::chunk::size> blocks_;
    bool mesh_valid_;
    
public:
	explicit Chunk(ChunkID id, glm::ivec2 world_coords);

    Block& BlockAt(const glm::ivec3& coords, bool check_index = false);

    const Block& BlockAt(const glm::ivec3& coords, bool check_index = false) const;

    glm::ivec3 Pos(int index) const;

    bool IsValidIndex(const glm::ivec3& coords) const;

    Block NeighborAt(const glm::ivec3& coords, Direction dir) const;

    Block& NeighborRefAt(const glm::ivec3& coords, Direction dir);

    // Getters
    ChunkID Id() const noexcept { return id_; }
    const glm::ivec2& WorldCoords() const noexcept { return world_coords_; }
    const std::array<Block, constants::chunk::size>& Blocks() const noexcept { return blocks_; }
    bool MeshValid() const noexcept { return mesh_valid_; }

    // Setters
    void SetMeshValid(bool mesh_valid) { mesh_valid_ = mesh_valid; }

private:
    int Index(const glm::ivec3& coords) const;
};

#endif // CHUNK_HPP
