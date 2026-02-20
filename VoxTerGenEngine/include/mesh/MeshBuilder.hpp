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

template <typename Fnc> 
concept BlockQuery = 
	std::invocable<Fnc, const glm::ivec3&> &&
	std::convertible_to<std::invoke_result_t<Fnc, const glm::ivec3&>, Block>;

class MeshBuilder
{
public:
	static Mesh BuildMeshNaive(const Chunk& chunk, BlockQuery auto&& block_query);
	
	static Mesh BuildMeshGreedy(const Chunk& chunk, BlockQuery auto&& block_query);

private:
	static void SaveQuadMesh(const Chunk& chunk, const glm::ivec3& block_coords, Direction dir, Mesh& chunk_mesh);

	static uint8_t GetQuadMaterial(BlockType block_type, Direction dir);

	static Mesh BuildAxisMesh(const Chunk& chunk, MajorAxis axis, BlockQuery auto&& world_block_query);
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
	// |X|X|X|A|A|A|
	// |X|X|X|X|A|A|
	// -------------
		bool merging = false;
		
		for (int v = 0; v < cross_1_size; ++v)
		{
			for (int u = 0; u < cross_2_size; ++u)
			{
				const MaskCell& cell = slice_mask[v * cross_1_size + u];

				if (cell.processed_ || cell.block_type_ == BlockType::Air)
				{
					// IF I FOUND SOME VERTICES/INDICES BEFORE, END SEARCHING, PRODUCE VERTICES AND INDICES AND SAVE
					continue;
				}
			}	
		}
		// merge all that can be merged
		// save the vertices & indices to output mesh
	}

	return axis_mesh;
}

#endif // MESH_BUILDER_HPP