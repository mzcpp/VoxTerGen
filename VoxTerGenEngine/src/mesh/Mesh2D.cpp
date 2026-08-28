#include "mesh/Mesh2D.hpp"
#include "mesh/Vertex.hpp"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <cstdint>

void Mesh2D::AddVertex(glm::vec3 position, glm::vec2 uv)
{
	vertices_.emplace_back(position, uv);
}

void Mesh2D::AddVertex(Vertex2D vertex)
{
	vertices_.push_back(std::move(vertex));
}

void Mesh2D::AddIndex(std::uint32_t index)
{
	indices_.emplace_back(index);
}