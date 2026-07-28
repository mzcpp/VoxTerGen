#ifndef MESH_HPP
#define MESH_HPP

#include "mesh/Vertex.hpp"

#include <vector>
#include <cstdint>

class Mesh
{
private:
	std::vector<Vertex> vertices_;
	std::vector<std::uint32_t> indices_;

public:
	void AddVertex(glm::vec3 position, std::uint8_t normal, glm::vec2 uv, std::uint8_t material);

	void AddVertex(Vertex vertex);

	void AddIndex(std::uint32_t index);
	
	// Getters
	const std::vector<Vertex>& Vertices() const noexcept { return vertices_; }
	const std::vector<std::uint32_t>& Indices() const noexcept { return indices_; }
};

#endif // MESH_HPP

