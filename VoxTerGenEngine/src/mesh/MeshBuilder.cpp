#include "mesh/MeshBuilder.hpp"
#include "mesh/Vertex.hpp"
#include "mesh/Mesh.hpp"
#include "world/ChunkManager.hpp"
#include "world/Chunk.hpp"
#include "render/Material.hpp"
#include "utils/Logger.hpp"
#include "core/Direction.hpp"
#include "utils/Constants.hpp"

#include <glm/vec3.hpp>

#include <array>
#include <cassert>

Mesh MeshBuilder::BuildUnitCubeMesh(BlockType block_type, glm::vec3 origin_offset)
{
	Mesh unit_cube_mesh;


	for (Direction dir : AllDirections())
    {
		CreateMeshIndices(unit_cube_mesh);
        CreateMeshVertices(block_type, dir, origin_offset, unit_cube_mesh);
    }

	return unit_cube_mesh;
}

void MeshBuilder::CreateMeshIndices(Mesh& chunk_mesh)
{
	for (std::uint32_t i : { 0, 1, 2, 1, 3, 2 })
	{
		chunk_mesh.AddIndex(i + static_cast<std::uint32_t>(chunk_mesh.Vertices().size()));
	}
}

void MeshBuilder::CreateMeshVertices(BlockType type, Direction dir, glm::vec3 origin_offset, Mesh& chunk_mesh)
{
	for (int i = 0; i < 4; ++i)
	{
		Vertex vertex;

		if (dir == Direction::PosX)
		{
			// 1, 0, 1
			// 1, 0, 0
			// 1, 1, 1
			// 1, 1, 0
			vertex.position_.x = 1.0f;
			vertex.position_.y = static_cast<float>((i / 2) % 2 != 0);
			vertex.position_.z = static_cast<float>(i % 2 == 0);
			vertex.normal_.x = 1.0f;
		}
		else if (dir == Direction::NegX)
		{
			// 0, 0, 0
			// 0, 0, 1
			// 0, 1, 0
			// 0, 1, 1
			vertex.position_.x = 0.0f;
			vertex.position_.y = static_cast<float>((i / 2) % 2 != 0);
			vertex.position_.z = static_cast<float>(i % 2 != 0);
			vertex.normal_.x = -1.0f;
		}
		else if (dir == Direction::PosY)
		{
			// 0, 1, 1
			// 1, 1, 1
			// 0, 1, 0
			// 1, 1, 0
			vertex.position_.x = static_cast<float>(i % 2 != 0);
			vertex.position_.y = 1.0f;
			vertex.position_.z = static_cast<float>((i / 2) % 2 == 0);
			vertex.normal_.y = 1.0f;
		}
		else if (dir == Direction::NegY)
		{
			// 1, 0, 1
			// 0, 0, 1
			// 1, 0, 0
			// 0, 0, 0
			vertex.position_.x = static_cast<float>(i % 2 == 0);
			vertex.position_.y = 0.0f;
			vertex.position_.z = static_cast<float>((i / 2) % 2 == 0);
			vertex.normal_.y = -1.0f;
		}
		else if (dir == Direction::PosZ)
		{
			// 0, 0, 1
			// 1, 0, 1
			// 0, 1, 1
			// 1, 1, 1
			vertex.position_.x = static_cast<float>(i % 2 != 0);
			vertex.position_.y = static_cast<float>((i / 2) % 2 != 0);
			vertex.position_.z = 1.0f;
			vertex.normal_.z = 1.0f;
		}
		else if (dir == Direction::NegZ)
		{
			// 1, 0, 0
			// 0, 0, 0
			// 1, 1, 0
			// 0, 1, 0
			vertex.position_.x = static_cast<float>(i % 2 == 0);
			vertex.position_.y = static_cast<float>((i / 2) % 2 != 0);
			vertex.position_.z = 0.0f;
			vertex.normal_.z = -1.0f;
		}

		vertex.position_ += origin_offset;
		vertex.uv_ = { static_cast<float>(i % 2 != 0), static_cast<float>((i / 2) % 2 != 0) };
		vertex.material_ = GetQuadMaterial(type, dir);

		chunk_mesh.AddVertex(vertex);
	}
}

void MeshBuilder::SaveQuadMesh(glm::ivec2 chunk_world_coords, BlockType type, glm::ivec3 block_rel_coords, Direction dir, Mesh& chunk_mesh)
{
	const glm::vec3 block_abs_pos = { 
		static_cast<float>(chunk_world_coords.x * constants::chunk::width + block_rel_coords.x), 
		static_cast<float>(block_rel_coords.y), 
		static_cast<float>(chunk_world_coords.y * constants::chunk::depth + block_rel_coords.z) 
	};

	CreateMeshIndices(chunk_mesh);
	CreateMeshVertices(type, dir, block_abs_pos, chunk_mesh);
	
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
	for (std::uint32_t i : { 0, 1, 2, 1, 3, 2 })
	{
		chunk_mesh.AddIndex(i + static_cast<std::uint32_t>(chunk_mesh.Vertices().size()));
	}

	glm::vec3 vertex_position = { 0.0f, 0.0f, 0.0f };
	
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
				vertex_position = { major_axis_index + 1, y_pos, x_pos };
			}
			else if (major_axis == MajorAxis::Y)
			{
				vertex_position = { x_pos, major_axis_index + 1, y_pos };
			}
			else
			{
				vertex_position = { x_pos, y_pos, major_axis_index + 1 };
			}

			const glm::vec3 normal = DirToNormal(first_merged_cell.dir_);
			const glm::vec2 uv = { static_cast<float>(j * merged_quad.width_), static_cast<float>(i * merged_quad.height_) };
			std::uint8_t material = GetQuadMaterial(first_merged_cell.block_type_, first_merged_cell.dir_);

			chunk_mesh.AddVertex(vertex_position, normal, uv, material);
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