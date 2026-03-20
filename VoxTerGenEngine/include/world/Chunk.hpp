#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "world/Block.hpp"
#include "utils/Constants.hpp"
#include "utils/Logger.hpp"
#include "mesh/Mesh.hpp"
#include "render/MeshRenderer.hpp"
#include "core/Direction.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <functional>
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
    bool mesh_invalid_;

public:
	explicit Chunk(glm::ivec2 world_coords);

    Block& BlockAt(const glm::ivec3& coords, bool check_index = false);

    const Block& BlockAt(const glm::ivec3& coords, bool check_index = false) const;

    glm::ivec3 Pos(int index) const;

    bool IsValidIndex(const glm::ivec3& coords) const;

    Block NeighborAt(const glm::ivec3& coords, Direction dir) const;

    Block& NeighborRefAt(const glm::ivec3& coords, Direction dir);

    int BlockCount() const { return constants::chunk::size; }

    const glm::ivec2& WorldCoords() const { return world_coords_; }

    const std::array<Block, constants::chunk::size>& Blocks() const { return blocks_; }

    const Mesh& Mesh() const { return *mesh_; }

    void SetMesh(std::unique_ptr<class Mesh> mesh) { mesh_ = std::move(mesh); }

private:
    int Index(const glm::ivec3& coords) const;
};

#endif // CHUNK_HPP
