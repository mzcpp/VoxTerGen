#include "mesh/MeshBuilder.hpp"
#include "mesh/Vertex.hpp"
#include "mesh/Mesh.hpp"
#include "world/ChunkManager.hpp"
#include "world/Chunk.hpp"
#include "render/Material.hpp"
#include "utils/Logger.hpp"
#include "core/Direction.hpp"
#include "utils/Constants.hpp"

#include <array>
#include <cassert>

void MeshBuilder::SaveQuadMesh(const glm::ivec2& chunk_world_coords, BlockType type, const glm::ivec3& block_coords, Direction dir, Mesh& chunk_mesh)
{
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
		quad_vertices[i].material_ = GetQuadMaterial(type, dir);
	}
	
	for (std::uint32_t i : { 0, 1, 2, 1, 3, 2 })
	{
		chunk_mesh.Indices().push_back(i + static_cast<std::uint32_t>(chunk_mesh.Vertices().size()));
	}

	chunk_mesh.Vertices().insert(chunk_mesh.Vertices().end(), quad_vertices.begin(), quad_vertices.end());
	assert(chunk_mesh.Vertices().size() % 4 == 0);
}

std::uint8_t MeshBuilder::GetQuadMaterial(BlockType block_type, Direction dir)
{
	switch (block_type)
	{
	case BlockType::Air:
		return static_cast<std::uint8_t>(Material::Air);
	case BlockType::Grass:
		if (dir == Direction::PosY)
		{
			return static_cast<std::uint8_t>(Material::GrassTop);
		}
		else if (dir == Direction::NegY)
		{
			return static_cast<std::uint8_t>(Material::Dirt);
		}
		else
		{
			return static_cast<std::uint8_t>(Material::GrassSide);
		}
	case BlockType::Dirt:
		return static_cast<std::uint8_t>(Material::Dirt);
	case BlockType::Stone:
		return static_cast<std::uint8_t>(Material::Stone);
	case BlockType::Water:
		return static_cast<std::uint8_t>(Material::Water);
	case BlockType::Sand:
		return static_cast<std::uint8_t>(Material::Sand);
	case BlockType::Snow:
		return static_cast<std::uint8_t>(Material::Snow);
	case BlockType::Bedrock:
		return static_cast<std::uint8_t>(Material::Bedrock);
	}

	assert(false);
	Logger::Log(LogLevel::ERROR, "GetQuadMaterial received an unknown type of Block!: block_type = {}", static_cast<std::uint8_t>(block_type));
	return static_cast<std::uint8_t>(Material::Air);
}

bool MeshBuilder::MaskCellsMergable(const MaskCell& first, const MaskCell& second)
{
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

void MeshBuilder::EmitVerticesAndIndices(MajorAxis major_axis, const MergedQuad& merged_quad, int major_axis_index, const MaskCell& first_merged_cell, Mesh& chunk_mesh)
{
	Vertex vertex;
	vertex.normal_ = DirToNormal(first_merged_cell.dir_);
	vertex.material_ = GetQuadMaterial(first_merged_cell.block_type_, first_merged_cell.dir_);

	for (std::uint32_t i : { 0, 1, 2, 1, 3, 2 })
	{
		chunk_mesh.Indices().push_back(i + static_cast<std::uint32_t>(chunk_mesh.Vertices().size()));
	}
	
	for (int i : { 0, 1 })
	{
		for (int j : { 0, 1 })
		{
			if ((major_axis == MajorAxis::X && static_cast<std::uint8_t>(first_merged_cell.dir_) % 2 == 0) || 
				(major_axis == MajorAxis::Y && static_cast<std::uint8_t>(first_merged_cell.dir_) % 2 == 0) || 
				(major_axis == MajorAxis::Z && static_cast<std::uint8_t>(first_merged_cell.dir_) % 2 != 0))
			{
				j = 1 - j;
			}

			const int x_pos = merged_quad.bottom_left_.x + (j * merged_quad.width_);
			const int y_pos = merged_quad.bottom_left_.y + (i * merged_quad.height_);

			if (major_axis == MajorAxis::X)
			{
				vertex.position_ = { major_axis_index + 1, y_pos, x_pos };
			}
			else if (major_axis == MajorAxis::Y)
			{
				vertex.position_ = { x_pos, major_axis_index + 1, y_pos };
			}
			else
			{
				vertex.position_ = { x_pos, y_pos, major_axis_index + 1 };
			}

			vertex.uv_ = GetTextureCoords({ j, i }, merged_quad, static_cast<Material>(vertex.material_));
			chunk_mesh.Vertices().push_back(vertex);
		}
	}
}

void MeshBuilder::MergeFacesAndEmitData(MajorAxis major_axis, int major_axis_index, int mask_width, int mask_height, std::vector<MaskCell>& slice_mask, Mesh& chunk_mesh)
{
	for (int y = 0; y < mask_height; ++y)
	{
		for (int x = 0; x < mask_width;)
		{
			const MaskCell& cell = slice_mask[y * mask_width + x];
			
			if (cell.block_type_ == BlockType::Air)
			{
				++x;
				continue;
			}

			int merged_quad_width = 1;

			while (x + merged_quad_width < mask_width && 
				MaskCellsMergable(cell, slice_mask[y * mask_width + (x + merged_quad_width)]))
			{
				++merged_quad_width;
			}

			int merged_quad_height = 1;
			bool done = false;

			while (y + merged_quad_height < mask_height && !done)
            {
                for (int k = 0; k < merged_quad_width; ++k)
                {
                    if (!MaskCellsMergable(cell, slice_mask[(y + merged_quad_height) * mask_width + x + k]))
                    {
                        done = true;
                        break;
                    }
                }

                if (!done)
				{
                    ++merged_quad_height;
				}
            }

			MergedQuad merged_quad;
			merged_quad.bottom_left_.x = x;
			merged_quad.bottom_left_.y = y;
			merged_quad.width_ = merged_quad_width;
			merged_quad.height_ = merged_quad_height;
			
			EmitVerticesAndIndices(major_axis, merged_quad, major_axis_index, cell, chunk_mesh);

			for (int dy = 0; dy < merged_quad_height; ++dy)
            {
                for (int dx = 0; dx < merged_quad_width; ++dx)
                {
                    slice_mask[(y + dy) * mask_width + (x + dx)].block_type_ = BlockType::Air;
                }
            }

			x += merged_quad_width;
		}	
	}
}

glm::ivec2 MeshBuilder::GetTextureCoords(const glm::ivec2& quad_coords, const MergedQuad& merged_quad, Material material)
{
	glm::ivec2 material_texture_start_coords = { 0.0f, 0.0f };
	const glm::ivec2 xy_delta = { 1.0f / constants::texture::atlas_columns, 1.0f / constants::texture::atlas_rows };

	switch (material)
	{
	case Material::Air:
	case Material::GrassTop:
		break;
	case Material::GrassSide:
		material_texture_start_coords.x = 1 * (xy_delta.x);
		break;
	case Material::Dirt:
		material_texture_start_coords.x = 1 * (xy_delta.x);
		material_texture_start_coords.y = 1 * (xy_delta.y);
		break;
	case Material::Stone:
		material_texture_start_coords.y = 1 * (xy_delta.y);
		break;
	case Material::Water:
		material_texture_start_coords.y = 3 * (xy_delta.y);
		break;
	case Material::Snow:
		material_texture_start_coords.x = 1 * (xy_delta.x);
		material_texture_start_coords.y = 2 * (xy_delta.y);
		break;
	case Material::Sand:
		material_texture_start_coords.y = 2 * (xy_delta.y);
		break;
	case Material::Bedrock:
		material_texture_start_coords.x = 1 * (xy_delta.x);
		material_texture_start_coords.y = 3 * (xy_delta.y);
		break;
	}

	material_texture_start_coords.x += quad_coords.x * (merged_quad.width_ * xy_delta.x);
	material_texture_start_coords.y += quad_coords.y * (merged_quad.height_ * xy_delta.y);

	return material_texture_start_coords;
}