#include "mesh/MeshBuilder.hpp"
#include "mesh/Vertex.hpp"
#include "mesh/Mesh.hpp"
#include "world/ChunkManager.hpp"
#include "world/Chunk.hpp"
#include "render/Material.hpp"
#include "utils/Logger.hpp"

#include <array>
#include <cassert>

void MeshBuilder::SaveQuadMesh(const Chunk& chunk, const glm::ivec3& block_coords, Direction dir, Mesh& chunk_mesh)
{
	const glm::ivec2& chunk_world_coords = chunk.WorldCoords();
	std::array<Vertex, 4> quad_vertices;
	
	float vertex_x = 0.0f;
	float vertex_y = 0.0f;
	float vertex_z = 0.0f;
	float normal_x = 0.0f;
	float normal_y = 0.0f;
	float normal_z = 0.0f;
	const float world_x = static_cast<float>(chunk_world_coords.x * constants::chunk::width + block_coords.x);
	const float world_y = static_cast<float>(block_coords.y);
	const float world_z = static_cast<float>(chunk_world_coords.y * constants::chunk::depth + block_coords.z);

	for (int i = 0; i < 4; ++i)
	{
		if (dir == Direction::PosX)
		{
			// 1, 0, 1
			// 1, 0, 0
			// 1, 1, 1
			// 1, 1, 0
			vertex_x = 1.0f;
			vertex_y = static_cast<float>((i / 2) % 2 != 0);
			vertex_z = static_cast<float>(i % 2 == 0);
			normal_x = 1.0f;
		}
		else if (dir == Direction::NegX)
		{
			// 0, 0, 0
			// 0, 0, 1
			// 0, 1, 0
			// 0, 1, 1
			vertex_x = 0.0f;
			vertex_y = static_cast<float>((i / 2) % 2 != 0);
			vertex_z = static_cast<float>(i % 2 != 0);
			normal_x = -1.0f;
		}
		else if (dir == Direction::PosY)
		{
			// 0, 1, 1
			// 1, 1, 1
			// 0, 1, 0
			// 1, 1, 0
			vertex_x = static_cast<float>(i % 2 != 0);
			vertex_y = 1.0f;
			vertex_z = static_cast<float>((i / 2) % 2 == 0);
			normal_y = 1.0f;
		}
		else if (dir == Direction::NegY)
		{
			// 1, 0, 1
			// 0, 0, 1
			// 1, 0, 0
			// 0, 0, 0
			vertex_x = static_cast<float>(i % 2 == 0);
			vertex_y = 0.0f;
			vertex_z = static_cast<float>((i / 2) % 2 == 0);
			normal_y = -1.0f;
		}
		else if (dir == Direction::PosZ)
		{
			// 0, 0, 1
			// 1, 0, 1
			// 0, 1, 1
			// 1, 1, 1
			vertex_x = static_cast<float>(i % 2 != 0);
			vertex_y = static_cast<float>((i / 2) % 2 != 0);
			vertex_z = 1.0f;
			normal_z = 1.0f;
		}
		else if (dir == Direction::NegZ)
		{
			// 1, 0, 0
			// 0, 0, 0
			// 1, 1, 0
			// 0, 1, 0
			vertex_x = static_cast<float>(i % 2 == 0);
			vertex_y = static_cast<float>((i / 2) % 2 != 0);
			vertex_z = 0.0f;
			normal_z = -1.0f;
		}

		quad_vertices[i].position_ = { vertex_x + world_x, vertex_y + world_y, vertex_z + world_z };
		quad_vertices[i].normal_ = { normal_x, normal_y, normal_z };
		quad_vertices[i].uv_ = { static_cast<float>(i % 2 != 0), static_cast<float>((i / 2) % 2 != 0) };
		quad_vertices[i].material_ = GetQuadMaterial(chunk.BlockAt(block_coords.x, block_coords.y, block_coords.z).Type(), dir);
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

bool MeshBuilder::MaskCellsMergable(const MaskCell& first, const MaskCell& second)
{
	if (first.processed_ || second.processed_)
	{
		return false;
	}

	if (first.block_type_ == BlockType::Air || second.block_type_ == BlockType::Air)
	{
		return false;
	}

	if (first.block_type_ != second.block_type_)
	{
		return false;
	}

	if (first.dir_ != second.dir_)
	{
		return false;
	}

	if (first.sun_light_ != second.sun_light_)
	{
		return false;
	}

	if (first.block_light_ != second.block_light_)
	{
		return false;
	}

	return true;
}

bool MeshBuilder::MergeWithRowAbove(int start_x, int end_x, int y, int height, const MaskCell& cell_to_match, const std::vector<MaskCell>& slice_mask, MergedQuad& merged_quad)
{
	if (start_x >= end_x || start_x < 0 || y >= height || y < 0)
	{
		return false;
	}

	for (int x = start_x; x < end_x; ++x)
	{
		const MaskCell& cell = slice_mask[y * height + x];

		if (!MaskCellsMergable(cell_to_match, cell))
		{
			return false;
		}
	}

	++merged_quad.height_;
	return true;
}