#include "mesh/Mesh.hpp"

void Mesh::AddVertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv, std::uint8_t material)
{
	vertices_.emplace_back(position, normal, uv, material);
}

void Mesh::AddIndex(std::uint32_t index)
{
	indices_.emplace_back(index);
}