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

namespace
{
    struct ChunkMeshReady
    {
        std::uint64_t id_;
        
    };

    struct ChunkDestroyed
    {
        std::uint64_t id_;
    };
}

using ChunkEvent = std::variant<ChunkMeshReady, ChunkDestroyed>;

class Chunk
{
private:
	static constexpr std::array<glm::ivec3, 6> neighbor_offsets_
	{
		glm::ivec3{ 1, 0, 0 }, glm::ivec3{ -1, 0, 0 },
		glm::ivec3{ 0, 1, 0 }, glm::ivec3{ 0, -1, 0 },
		glm::ivec3{ 0, 0, 1 }, glm::ivec3{ 0, 0, -1 }
	};

	glm::ivec2 world_coords_;
	std::array<Block, constants::chunk::size> blocks_;
    std::unique_ptr<Mesh> mesh_;
    bool mesh_valid_;
    bool mesh_needs_upload_;


public:
	explicit Chunk(glm::ivec2 world_coords);

    Block& BlockAt(const glm::ivec3& coords, bool check_index = false);

    const Block& BlockAt(const glm::ivec3& coords, bool check_index = false) const;

    glm::ivec3 Pos(int index) const;

    bool IsValidIndex(const glm::ivec3& coords) const;

    Block NeighborAt(const glm::ivec3& coords, Direction dir) const;

    Block& NeighborRefAt(const glm::ivec3& coords, Direction dir);

    void ReleaseMeshData();

    // Getters
    const glm::ivec2& WorldCoords() const noexcept { return world_coords_; }
    const std::array<Block, constants::chunk::size>& Blocks() const noexcept { return blocks_; }
    const Mesh* Mesh() const noexcept { return mesh_.get(); }
    bool MeshValid() const noexcept { return mesh_valid_; }
    bool MeshNeedsUpload() const noexcept { return mesh_needs_upload_; }

    // Setters
    void SetMesh(std::unique_ptr<class Mesh> mesh) { mesh_ = std::move(mesh); }
    void SetMeshValid(bool mesh_valid) { mesh_valid_ = mesh_valid; }
    void SetMeshNeedsUpload(bool mesh_needs_upload) { mesh_needs_upload_ = mesh_needs_upload; }

private:
    int Index(const glm::ivec3& coords) const;
};

#endif // CHUNK_HPP
