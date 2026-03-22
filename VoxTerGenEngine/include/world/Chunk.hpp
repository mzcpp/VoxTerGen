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
    std::unique_ptr<GpuMesh> gpu_mesh_;
    bool mesh_invalid_;

public:
	explicit Chunk(glm::ivec2 world_coords);

    Block& BlockAt(const glm::ivec3& coords, bool check_index = false);

    const Block& BlockAt(const glm::ivec3& coords, bool check_index = false) const;

    glm::ivec3 Pos(int index) const;

    bool IsValidIndex(const glm::ivec3& coords) const;

    Block NeighborAt(const glm::ivec3& coords, Direction dir) const;

    Block& NeighborRefAt(const glm::ivec3& coords, Direction dir);

    void UploadMeshData() const;

    // Getters
    const glm::ivec2& WorldCoords() const noexcept { return world_coords_; }
    const std::array<Block, constants::chunk::size>& Blocks() const noexcept { return blocks_; }
    const Mesh& Mesh() const { return *mesh_; }
    const GpuMesh& GpuMesh() const { return *gpu_mesh_; }
    bool MeshInvalid() const { return mesh_invalid_; }

    // Setters
    void SetMesh(std::unique_ptr<class Mesh> mesh) { mesh_ = std::move(mesh); }
    void SetMeshInvalid(bool mesh_invalid) { mesh_invalid_ = mesh_invalid; }

private:
    int Index(const glm::ivec3& coords) const;
};

#endif // CHUNK_HPP
