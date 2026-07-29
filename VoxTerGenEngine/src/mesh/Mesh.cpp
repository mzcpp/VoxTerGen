#include "mesh/Mesh.hpp"

void Mesh::AddVertex(glm::vec3 position, std::uint8_t normal, glm::vec2 uv, std::uint8_t material)
{
	vertices_.emplace_back(position, normal, uv, material);
}

void Mesh::AddVertex(Vertex vertex)
{
	vertices_.push_back(std::move(vertex));
}

void Mesh::AddIndex(std::uint32_t index)
{
	indices_.emplace_back(index);
}