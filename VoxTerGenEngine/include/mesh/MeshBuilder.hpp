#ifndef MESH_BUILDER_HPP
#define MESH_BUILDER_HPP

#include "mesh/Mesh.hpp"
#include "world/Chunk.hpp"
#include "world/Block.hpp"
#include "utils/constants.hpp"
#include "core/Direction.hpp"
#include "render/Material.hpp"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include <concepts>
#include <cstdint>
#include <vector>

struct MaskCell
{
	BlockType block_type_;
	Direction dir_;
	std::uint8_t sun_light_;
	std::uint8_t block_light_;
};

struct MergedQuad
{
	glm::ivec2 bottom_left_;
	int width_;
	int height_;
};

template <typename Fnc> 
concept BlockQuery = std::invocable<Fnc, const glm::ivec3&> &&
	std::convertible_to<std::invoke_result_t<Fnc, const glm::ivec3&>, Block>;

class MeshBuilder final
{
public:
	MeshBuilder() = delete;
	MeshBuilder(const MeshBuilder&) = delete;
	MeshBuilder& operator=(const MeshBuilder&) = delete;

	static Mesh BuildMeshNaive(const glm::ivec2& chunk_world_coords, BlockQuery auto&& world_block_query);
	
	static Mesh BuildMeshGreedy(BlockQuery auto&& world_block_query);

private:
	static void SaveQuadMesh(const glm::ivec2& chunk_world_coords, BlockType type, const glm::ivec3& block_coords, Direction dir, Mesh& chunk_mesh);

	static std::uint8_t GetQuadMaterial(BlockType block_type, Direction dir);

	static void BuildAxisMesh(MajorAxis major_axis, BlockQuery auto&& world_block_query, Mesh& chunk_mesh);

	static bool MaskCellsMergable(const MaskCell& first, const MaskCell& second);

	static void BuildSliceMask(MajorAxis major_axis, int major_axis_index, int major_axis_size, int cross_axis_1_size, int cross_axis_2_size, BlockQuery auto&& world_block_query, std::vector<MaskCell>& slice_mask);
	
	static void EmitVerticesAndIndices(MajorAxis major_axis, const MergedQuad& merged_quad, int major_axis_index, const MaskCell& first_merged_cell, Mesh& chunk_mesh);

	static void MergeFacesAndEmitData(MajorAxis major_axis, int major_axis_index, int mask_width, int mask_height, std::vector<MaskCell>& slice_mask, Mesh& chunk_mesh);
};

Mesh MeshBuilder::BuildMeshNaive(const glm::ivec2& chunk_world_coords, BlockQuery auto&& world_block_query)
{
	Mesh chunk_mesh;

	for (int z = 0; z < constants::chunk::depth; ++z)
	{
		for (int y = 0; y < constants::chunk::height; ++y)
		{
			for (int x = 0; x < constants::chunk::width; ++x)
			{
				if (!world_block_query({ x, y, z }).IsSolid())
				{
					continue;
				}

				for (Direction dir : AllDirections())
				{
					const glm::ivec3 neighbor_coords = NeighborCoords({ x, y, z }, dir);

					if (world_block_query(neighbor_coords).IsSolid())
					{
						continue;
					}

					SaveQuadMesh(chunk_world_coords, world_block_query({ x, y, z }).Type(), { x, y, z }, dir, chunk_mesh);
				}
			}
		}
	}

	return chunk_mesh;
}

Mesh MeshBuilder::BuildMeshGreedy(BlockQuery auto&& world_block_query)
{
	Mesh chunk_mesh;

	 for (MajorAxis axis : AllAxes())
	 {
	 	BuildAxisMesh(axis, world_block_query, chunk_mesh);
	 }

	return chunk_mesh;
}

void MeshBuilder::BuildAxisMesh(MajorAxis major_axis, BlockQuery auto&& world_block_query, Mesh& chunk_mesh)
{
	int major_axis_size = 0;
	int cross_axis_1_size = 0;
	int cross_axis_2_size = 0;

	if (major_axis == MajorAxis::X)
	{
		major_axis_size = constants::chunk::width;
		cross_axis_1_size = constants::chunk::height;
		cross_axis_2_size = constants::chunk::depth;
	}
	else if (major_axis == MajorAxis::Y)
	{
		major_axis_size = constants::chunk::height;
		cross_axis_1_size = constants::chunk::depth;
		cross_axis_2_size = constants::chunk::width;
	}
	else
	{
		major_axis_size = constants::chunk::depth;
		cross_axis_1_size = constants::chunk::height;
		cross_axis_2_size = constants::chunk::width;
	}
	
	std::vector<MaskCell> slice_mask;
	slice_mask.resize(cross_axis_1_size * cross_axis_2_size);

	for (int major_axis_index = -1; major_axis_index < major_axis_size; ++major_axis_index)
	{
		BuildSliceMask(major_axis, major_axis_index, major_axis_size, cross_axis_1_size, cross_axis_2_size, world_block_query, slice_mask);
		MergeFacesAndEmitData(major_axis, major_axis_index, cross_axis_2_size, cross_axis_1_size, slice_mask, chunk_mesh);
	}
}

void MeshBuilder::BuildSliceMask(MajorAxis major_axis, int major_axis_index, int major_axis_size, int cross_axis_1_size, int cross_axis_2_size, BlockQuery auto&& world_block_query, std::vector<MaskCell>& slice_mask)
{
	for (int cross_axis_1_index = 0; cross_axis_1_index < cross_axis_1_size; ++cross_axis_1_index)
	{
		for (int cross_axis_2_index = 0; cross_axis_2_index < cross_axis_2_size; ++cross_axis_2_index)
		{
			glm::ivec3 left_query_coords = { 0, 0, 0 };
			glm::ivec3 right_query_coords = { 0, 0, 0 };

			if (major_axis == MajorAxis::X)
			{
				left_query_coords = { major_axis_index, cross_axis_1_index, cross_axis_2_index };
				right_query_coords = { major_axis_index + 1, cross_axis_1_index, cross_axis_2_index };
			}
			else if (major_axis == MajorAxis::Y)
			{
				left_query_coords = { cross_axis_2_index, major_axis_index, cross_axis_1_index };
				right_query_coords = { cross_axis_2_index, major_axis_index + 1, cross_axis_1_index };
			}
			else
			{
				left_query_coords = { cross_axis_2_index, cross_axis_1_index, major_axis_index };
				right_query_coords = { cross_axis_2_index, cross_axis_1_index, major_axis_index + 1 };
			}

			const Block& left_block = world_block_query(left_query_coords);
			const Block& right_block = world_block_query(right_query_coords);

			const bool left_block_inside = major_axis_index != -1;
			const bool right_block_inside = (major_axis_index + 1) != major_axis_size;

			const bool render_left = left_block_inside && left_block.ShouldRenderFace(right_block);
			const bool render_right = right_block_inside && right_block.ShouldRenderFace(left_block);

			MaskCell mask_cell = { BlockType::Air, Direction::PosX, 0, 0 };

			if (render_left)
			{
				mask_cell.block_type_ = left_block.Type();
				mask_cell.dir_ = ToDirection(major_axis, true);
				mask_cell.sun_light_ = left_block.SunLight();
				mask_cell.block_light_ = left_block.BlockLight();
			}
			else if (render_right)
			{
				mask_cell.block_type_ = right_block.Type();
				mask_cell.dir_ = ToDirection(major_axis, false);
				mask_cell.sun_light_ = right_block.SunLight();
				mask_cell.block_light_ = right_block.BlockLight();
			}

			slice_mask[cross_axis_1_index * cross_axis_2_size + cross_axis_2_index] = mask_cell;
		}
	}
}

#endif // MESH_BUILDER_HPP