#include "mesh/Mesh3D.hpp"
#include "mesh/Vertex.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cstdint>

void Mesh3D::AddVertex(glm::vec3 position, std::uint8_t normal, glm::vec2 uv, std::uint8_t material)
{
	vertices_.emplace_back(position, normal, uv, material);
}

void Mesh3D::AddVertex(Vertex3D vertex)
{
	vertices_.push_back(std::move(vertex));
}

void Mesh3D::AddIndex(std::uint32_t index)
{
	indices_.emplace_back(index);
}