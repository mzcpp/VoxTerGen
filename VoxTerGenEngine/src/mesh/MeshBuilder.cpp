#include "mesh/MeshBuilder.hpp"
#include "mesh/Vertex.hpp"
#include "mesh/Mesh.hpp"
#include "world/ChunkManager.hpp"
#include "world/Chunk.hpp"
#include "render/Material.hpp"
#include "utils/Logger.hpp"
#include "core/Direction.hpp"

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
	}

	assert(false);
	Logger::Log(LogLevel::ERROR, "GetQuadMaterial received an unknown type of Block!: block_type = {}", static_cast<std::uint8_t>(block_type));
	return static_cast<std::uint8_t>(Material::Air);
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

bool MeshBuilder::MergeWithRowAbove(int start_x, int end_x, int y, int height, int width, const MaskCell& cell_to_match, const std::vector<MaskCell>& slice_mask, MergedQuad& merged_quad)
{
	assert(slice_mask.size() == static_cast<std::size_t>(width * height));

	if (start_x >= end_x || start_x < 0 || end_x > width || y < 0 || y >= height)
	{
		return false;
	}

	for (int x = start_x; x < end_x; ++x)
	{
		const MaskCell& cell = slice_mask[y * width + x];

		if (!MaskCellsMergable(cell_to_match, cell))
		{
			return false;
		}
	}

	++merged_quad.height_;
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

			const int uu = merged_quad.bottom_left_.x + (j * merged_quad.width_);
			const int vv = merged_quad.bottom_left_.y + (i * merged_quad.height_);

			if (major_axis == MajorAxis::X)
			{
				vertex.position_ = { major_axis_index + 1, vv, uu };
			}
			else if (major_axis == MajorAxis::Y)
			{
				vertex.position_ = { uu, major_axis_index + 1, vv };
			}
			else
			{
				vertex.position_ = { uu, vv, major_axis_index + 1 };
			}

			vertex.uv_ = { j * merged_quad.width_, i * merged_quad.height_ };
			chunk_mesh.Vertices().push_back(vertex);
		}
	}
}

void MeshBuilder::MergeFacesAndEmitData(MajorAxis major_axis, int major_axis_index, int cross_axis_1_size, int cross_axis_2_size, std::vector<MaskCell>& slice_mask, Mesh& chunk_mesh)
{
	bool merging = false;
	MergedQuad merged_quad = { { 0, 0 }, 1, 1 };

	for (int v = 0; v < cross_axis_1_size; ++v)
	{
		for (int u = 1; u < cross_axis_2_size; ++u)
		{
			MaskCell& cell = slice_mask[(v * cross_axis_2_size) + (u - 1)];
			MaskCell& next_cell = slice_mask[(v * cross_axis_2_size) + u];

			if (u == 1 && cell.block_type_ != BlockType::Air)
			{
				merging = true;
				merged_quad.bottom_left_ = { u - 1, v };
			}

			if (cell.processed_)
			{
				if (!next_cell.processed_)
				{
					merging = true;
					merged_quad.bottom_left_ = { u, v };
				}

				continue;
			}

			const bool at_last_cell = u == cross_axis_2_size - 1;
			const bool cells_mergable = MaskCellsMergable(cell, next_cell);

			if (!merging && cells_mergable)
			{
				merging = true;
				merged_quad.bottom_left_ = { u - 1, v };
			}

			if (merging && (!cells_mergable || at_last_cell))
			{
				if (at_last_cell && cells_mergable)
				{
					++merged_quad.width_;
				}

				int height = v;

				while (MergeWithRowAbove(merged_quad.bottom_left_.x, merged_quad.width_, height + 1, cross_axis_1_size, cross_axis_2_size, cell, slice_mask, merged_quad))
				{
					++height;
				}

				merging = false;

				for (int merged_y = merged_quad.bottom_left_.y; merged_y < merged_quad.height_; ++merged_y)
				{
					for (int merged_x = merged_quad.bottom_left_.x; merged_x < merged_quad.width_; ++merged_x)
					{
						slice_mask[merged_y * cross_axis_2_size + merged_x].processed_ = true;
					}
				}

				const MaskCell& first_merged_cell = slice_mask[merged_quad.bottom_left_.y * cross_axis_2_size + merged_quad.bottom_left_.x];
				EmitVerticesAndIndices(major_axis, merged_quad, major_axis_index, first_merged_cell, chunk_mesh);

				if (!at_last_cell)
				{
					merged_quad.bottom_left_ = { merged_quad.bottom_left_.x + merged_quad.width_, v };
				}

				merged_quad.width_ = 1;
				merged_quad.height_ = 1;
			}
			else
			{
				++merged_quad.width_;
			}
		}
	}
}
