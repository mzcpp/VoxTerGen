#ifndef MESH_3D_HPP
#define MESH_3D_HPP

#include "mesh/Vertex.hpp"

#include <cstdint>
#include <vector>

class Mesh3D
{
private:
	std::vector<Vertex3D> vertices_;
	std::vector<std::uint32_t> indices_;

public:
	void AddVertex(glm::vec3 position, std::uint8_t normal, glm::vec2 uv, std::uint8_t material);

	void AddVertex(Vertex3D vertex);

	void AddIndex(std::uint32_t index);
	
	// Getters
	const std::vector<Vertex3D>& Vertices() const noexcept { return vertices_; }
	const std::vector<std::uint32_t>& Indices() const noexcept { return indices_; }
};

#endif // MESH_3D_HPP

