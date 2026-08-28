#ifndef MESH_HPP
#define MESH_HPP

#include "mesh/Vertex.hpp"

#include <cstdint>
#include <vector>

class Mesh2D
{
private:
	std::vector<Vertex2D> vertices_;
	std::vector<std::uint32_t> indices_;

public:
	void AddVertex(glm::vec3 position, glm::vec2 uv);

	void AddVertex(Vertex2D vertex);

	void AddIndex(std::uint32_t index);
	
	// Getters
	const std::vector<Vertex2D>& Vertices() const noexcept { return vertices_; }
	const std::vector<std::uint32_t>& Indices() const noexcept { return indices_; }
};

#endif // MESH_HPP

