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
	std::uint8_t block_type_;
	Direction dir_;
	std::uint8_t light_;
};

template <typename Fnc> 
concept BlockQuery = 
	std::invocable<Fnc, const glm::ivec3&> &&
	std::convertible_to<std::invoke_result_t<Fnc, const glm::ivec3&>, Block>;

class MeshBuilder
{
private:

public:
	static Mesh BuildMeshNaive(const Chunk& chunk, BlockQuery auto&& block_query);
	
	static Mesh BuildMeshGreedy(const Chunk& chunk, BlockQuery auto&& block_query);

private:
	static void SaveQuadMesh(const Chunk& chunk, const glm::ivec3& block_coords, Direction dir, Mesh& chunk_mesh);

	static uint8_t GetQuadMaterial(BlockType block_type, Direction dir);
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
	std::vector<MaskCell> mask(constants::chunk::height * constants::chunk::depth);

	for (int x_boundary = -1; x_boundary < constants::chunk::width; ++x_boundary)
	{
		//  | | | | | | |
		// -|0|1|2|3|4|5|-
		// -|0|1|2|3|4|5|-
		// -|0|1|2|3|4|5|-
		// -|0|1|2|3|4|5|-
		//  | | | | | | |

		for (int y = 0; y < constants::chunk::height; ++y)
		{
			for (int z = 0; z < constants::chunk::depth; ++z)
			{
				// if x == width - 1 ===> needs to handle separately??? ugly if
				const Block& chunk_block = chunk.BlockAt(x_boundary + 1, y, z);
				const Block& neg_x_neighbor = world_block_query(chunk, x_boundary + 1, y, z, Direction::NegX);

				const bool should_render_chunk_face = chunk_block.ShouldRenderFace(neg_x_neighbor);
				const bool should_render_neighbor_face = neg_x_neighbor.ShouldRenderFace(chunk_block);

				if (should_render_chunk_face)
				{
					MaskCell cell;
					cell.block_type_ = chunk_block.Type();
					cell.light_ = 0;
					cell.dir_ = Direction::NegX;
				}

				// else if neighbor INSIDE THIS CHUNK && should_render_neighbor_face
				// emit +x face into mask
			}
		}
	}

	mask.clear();
	mask.resize(constants::chunk::depth * constants::chunk::width);

	for (int y_boundary = 0; y_boundary < constants::chunk::height + 1; ++y_boundary)
	{
		for (int z = 0; z < constants::chunk::depth; ++z)
		{
			for (int x = 0; x < constants::chunk::width; ++x)
			{

			}
		}
	}

	mask.clear();
	mask.resize(constants::chunk::height * constants::chunk::width);

	for (int z_boundary = 0; z_boundary < constants::chunk::depth + 1; ++z_boundary)
	{
		for (int y = 0; y < constants::chunk::height; ++y)
		{
			for (int x = 0; x < constants::chunk::width; ++x)
			{

			}
		}
	}

	return chunk_mesh;
}

#endif // MESH_BUILDER_HPP