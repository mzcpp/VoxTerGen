#include "mesh/MeshBuilder.hpp"
#include "mesh/Vertex.hpp"
#include "mesh/Mesh.hpp"
#include "world/ChunkManager.hpp"
#include "world/Chunk.hpp"
#include "render/Material.hpp"
#include "utils/Logger.hpp"

#include <array>
#include <cassert>

void MeshBuilder::SaveQuadMesh(const Chunk& chunk, int block_x, int block_y, int block_z, Direction dir, Mesh& chunk_mesh)
{
	const glm::ivec2& chunk_world_coords = chunk.WorldCoords();
	std::array<Vertex, 4> quad_vertices;
	
	float vertex_x = 0.0f;
	float vertex_y = 0.0f;
	float vertex_z = 0.0f;
	float normal_x = 0.0f;
	float normal_y = 0.0f;
	float normal_z = 0.0f;
	const float world_x = static_cast<float>(chunk_world_coords.x * Constants::Chunk::width + block_x);
	const float world_y = static_cast<float>(block_y);
	const float world_z = static_cast<float>(chunk_world_coords.y * Constants::Chunk::depth + block_z);

	for (int i = 0; i < 4; ++i)
	{
		// 1, 0, 1
		// 1, 0, 0
		// 1, 1, 1
		// 1, 1, 0
		if (dir == Direction::PosX)
		{
			vertex_x = 1.0f;
			vertex_y = static_cast<float>((i / 2) % 2 != 0);
			vertex_z = static_cast<float>(i % 2 == 0);
			normal_x = 1.0f;
		}
		// 0, 0, 0
		// 0, 0, 1
		// 0, 1, 0
		// 0, 1, 1
		else if (dir == Direction::NegX)
		{
			vertex_x = 0.0f;
			vertex_y = static_cast<float>((i / 2) % 2 != 0);
			vertex_z = static_cast<float>(i % 2 != 0);
			normal_x = -1.0f;
		}
		// 0, 1, 1
		// 1, 1, 1
		// 0, 1, 0
		// 1, 1, 0
		else if (dir == Direction::PosY)
		{
			vertex_x = static_cast<float>(i % 2 != 0);
			vertex_y = 1.0f;
			vertex_z = static_cast<float>((i / 2) % 2 == 0);
			normal_y = 1.0f;
		}
		// 1, 0, 1
		// 0, 0, 1
		// 1, 0, 0
		// 0, 0, 0
		else if (dir == Direction::NegY)
		{
			vertex_x = static_cast<float>(i % 2 == 0);
			vertex_y = 0.0f;
			vertex_z = static_cast<float>((i / 2) % 2 == 0);
			normal_y = -1.0f;
		}
		// 0, 0, 1
		// 1, 0, 1
		// 0, 1, 1
		// 1, 1, 1
		else if (dir == Direction::PosZ)
		{
			vertex_x = static_cast<float>(i % 2 != 0);
			vertex_y = static_cast<float>((i / 2) % 2 != 0);
			vertex_z = 1.0f;
			normal_z = 1.0f;
		}
		// 1, 0, 0
		// 0, 0, 0
		// 1, 1, 0
		// 0, 1, 0
		else if (dir == Direction::NegZ)
		{
			vertex_x = static_cast<float>(i % 2 == 0);
			vertex_y = static_cast<float>((i / 2) % 2 != 0);
			vertex_z = 0.0f;
			normal_z = -1.0f;
		}

		quad_vertices[i].position_ = { vertex_x + world_x, vertex_y + world_y, vertex_z + world_z };
		quad_vertices[i].normal_ = { normal_x, normal_y, normal_z };
		quad_vertices[i].uv_ = { static_cast<float>(i % 2 != 0), static_cast<float>((i / 2) % 2 != 0) };
		quad_vertices[i].material_ = GetQuadMaterial(chunk.BlockAt(block_x, block_y, block_z).Type(), dir);
	}
	
	const std::uint32_t vertices_saved_before = static_cast<std::uint32_t>(chunk_mesh.Vertices().size());
	chunk_mesh.Vertices().insert(chunk_mesh.Vertices().end(), quad_vertices.begin(), quad_vertices.end());
	assert(chunk_mesh.Vertices().size() % 4 == 0);

	for (std::uint32_t i : { 0, 1, 2, 1, 3, 2 })
	{
		chunk_mesh.Indices().push_back(i + vertices_saved_before);
	}
}

uint8_t MeshBuilder::GetQuadMaterial(BlockType block_type, Direction dir)
{
	switch (block_type)
	{
	case BlockType::Air:
		return static_cast<uint8_t>(Material::Air);
	case BlockType::Grass:
		if (dir == Direction::PosY)
		{
			return static_cast<uint8_t>(Material::GrassTop);
		}
		else if (dir == Direction::NegY)
		{
			return static_cast<uint8_t>(Material::Dirt);
		}
		else
		{
			return static_cast<uint8_t>(Material::GrassSide);
		}
	case BlockType::Dirt:
		return static_cast<uint8_t>(Material::Dirt);
	case BlockType::Stone:
		return static_cast<uint8_t>(Material::Stone);
	case BlockType::Water:
		return static_cast<uint8_t>(Material::Water);
	case BlockType::Sand:
		return static_cast<uint8_t>(Material::Sand);
	case BlockType::Snow:
		return static_cast<uint8_t>(Material::Snow);
	}

	assert(false);
	Logger::Log(LogLevel::ERROR, "GetQuadMaterial received an unknown type of Block!: block_type = {}", static_cast<std::uint8_t>(block_type));
	return static_cast<uint8_t>(Material::Air);
}