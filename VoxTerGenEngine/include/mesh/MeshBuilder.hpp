#ifndef MESH_BUILDER_HPP
#define MESH_BUILDER_HPP

#include "mesh/Mesh.hpp"
#include "world/Chunk.hpp"
#include "world/Block.hpp"
#include "utils/constants.hpp"

#include <concepts>
#include <cstdint>
#include <vector>

struct MaskCell
{
	BlockType block_type_;
	Direction dir_;
	std::uint8_t sun_light_;
	std::uint8_t block_light_;
	bool processed_;
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

class MeshBuilder
{
public:
	static Mesh BuildMeshNaive(const Chunk& chunk, BlockQuery auto&& world_block_query);
	
	static Mesh BuildMeshGreedy(const Chunk& chunk, BlockQuery auto&& world_block_query);

private:
	static void SaveQuadMesh(const Chunk& chunk, const glm::ivec3& block_coords, Direction dir, Mesh& chunk_mesh);

	static uint8_t GetQuadMaterial(BlockType block_type, Direction dir);

	static Mesh BuildAxisMesh(const Chunk& chunk, MajorAxis axis, BlockQuery auto&& world_block_query);

	static bool MaskCellsMergable(const MaskCell& first, const MaskCell& second);

	static bool MergeWithRowAbove(int start_x, int end_x, int y, int height, const MaskCell& cell_to_match, MergedQuad& merged_quad);
};

Mesh MeshBuilder::BuildMeshNaive(const Chunk& chunk, BlockQuery auto&& world_block_query)
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

					SaveQuadMesh(chunk, { x, y, z }, dir, chunk_mesh);
				}
			}
		}
	}

	return chunk_mesh;
}

Mesh MeshBuilder::BuildMeshGreedy(const Chunk& chunk, BlockQuery auto&& world_block_query)
{
	Mesh chunk_mesh;

	const Mesh x_axis_mesh = BuildAxisMesh(chunk, MajorAxis::X, world_block_query);
	const Mesh y_axis_mesh = BuildAxisMesh(chunk, MajorAxis::Y, world_block_query);
	const Mesh z_axis_mesh = BuildAxisMesh(chunk, MajorAxis::Z, world_block_query);

	// save the meshes to chunk_mesh;

	return chunk_mesh;
}

Mesh MeshBuilder::BuildAxisMesh(const Chunk& chunk, MajorAxis axis, BlockQuery auto&& world_block_query)
{
	Mesh axis_mesh;
	std::vector<MaskCell> slice_mask;

	int major_size = 0;
	int cross_1_size = 0;
	int cross_2_size = 0;

	if (axis == MajorAxis::X)
	{
		major_size = constants::chunk::width;
		cross_1_size = constants::chunk::height;
		cross_2_size = constants::chunk::depth;
	}
	else if (axis == MajorAxis::Y)
	{
		major_size = constants::chunk::height;
		cross_1_size = constants::chunk::depth;
		cross_2_size = constants::chunk::width;
	}
	else
	{
		major_size = constants::chunk::depth;
		cross_1_size = constants::chunk::height;
		cross_2_size = constants::chunk::width;
	}

	slice_mask.resize(cross_1_size * cross_2_size);
	
	// | | | | | | | | | | |
	// |4|5|0|1|2|3|4|5|0|1|
	// |4|5|0|1|2|3|4|5|0|1|
	// |4|5|0|1|2|3|4|5|0|1|
	// |4|5|0|1|2|3|4|5|0|1|
	// | | | | | | | | | | |

	for (int major = -1; major < major_size; ++major)
	{
		for (int cross_1 = 0; cross_1 < cross_1_size; ++cross_1)
		{
			for (int cross_2 = 0; cross_2 < cross_2_size; ++cross_2)
			{
				const Block& left_block = world_block_query({ major, cross_1, cross_2 });
				const Block& right_block = world_block_query({ major + 1, cross_1, cross_2 });
				
				const bool left_block_inside = major >= 0 && major < major_size;
				const bool right_block_inside = (major + 1) >= 0 && (major + 1) < major_size;
				
				const bool render_left = left_block_inside && left_block.ShouldRenderFace(right_block);
				const bool render_right = right_block_inside && right_block.ShouldRenderFace(left_block);

				MaskCell mask_cell;
				mask_cell.block_type_ = { BlockType::Air, 0, 0, Direction::PosX, false };

				if (render_left)
				{
					mask_cell.block_type_ = left_block.Type();
					mask_cell.sun_light_ = left_block.SunLight();
					mask_cell.block_light_ = left_block.BlockLight();
					mask_cell.dir_ = ToDirection(axis, true);
				}
				else if (render_right)
				{
					mask_cell.block_type_ = right_block.Type();
					mask_cell.sun_light_ = right_block.SunLight();
					mask_cell.block_light_ = right_block.BlockLight();
					mask_cell.dir_ = ToDirection(axis, false);
				}

				slice_mask[cross_1 * cross_1_size + cross_2] = mask_cell;
			}
		}

	// _____________
	// |O|O|O|O|O|O|
	// |O|O|X|O|O|A|
	// |A|X|X|A|A|A|
	// |X|X|X|X|X|A|
	// -------------
	// _____________
	// |O|O|O|O|O|O|
	// |X|X|X|O|O|C|
	// |X|X|X|C|B|B|
	// |X|X|R|C|B|B|
	// -------------
	// _____________
	// |O|O|O|O|O|O|
	// |X|A|C|C|C|C|
	// |X|A|B|C|C|C|
	// |A|X|X|C|C|C|
	// -------------
	// _____________
	// |O|O|O|O|O|O|
	// |X|A|C|C|C|C|
	// |X|X|B|C|C|C|
	// |X|X|X|C|C|C|
	// -------------
	// _____________
	// |O|O|O|O|O|O|
	// |X|X|X|O|A|A|
	// |X|X|X|C|C|C|
	// |X|X|X|C|C|C|
	// -------------
	//
	// cell processed, next NOT processed = merging = true and continue?

		bool merging = false;
		MergedQuad merged_quad = { { 0, 0 }, 1, 1 };
		
		for (int v = 0; v < cross_1_size; ++v)
		{
			for (int u = 1; u < cross_2_size; ++u)
			{
				MaskCell& cell = slice_mask[(v * cross_1_size) + (u - 1)];
				MaskCell& next_cell = slice_mask[(v * cross_1_size) + u];

				if (cell.processed_)
				{
					merging = !cell_next.processed_;
					continue;
				}

				if (cell.block_type_ == BlockType::Air && cell_next.block_type_ == BlockType::Air)
				{
					cell.processed_ = true;
					cell_next.processed_ = true;
					continue;
				}

				const bool last_cell = u == cross_2_size - 1;
				const bool cells_mergable = MaskCellsMergable(cell, next_cell);

				if (!merging && cells_mergable)
				{
					merging = true;
				}

				if (merging && (!cells_mergable || last_cell))
				{
					if (last_cell)
					{
						++merged_quad.width_;
					}
					
					int height = v;

					while (MergeWithRowAbove(merged_quad.bottom_left_.x, merged_quad.width_, height + 1, cross_1_size, next_cell, merged_quad))
					{
						++height;
					}

					merging = false;

					// MARK AS PROCESSED!!!!!!!!!!!!!!
					
					// Emmit vertices & indices here!


					if (u != cross_2_size - 1)
					{
						merged_quad.bottom_left_ = { merged_quad.bottom_left.x + merged_quad.width_, v };
					}
					else
					{
						merged_quad.bottom_left_ = { 0, v + 1 }; // WRONG
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

	return axis_mesh;
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

#endif // MESH_BUILDER_HPP