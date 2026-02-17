#ifndef MESH_BUILDER_HPP
#define MESH_BUILDER_HPP

#include "mesh/Mesh.hpp"
#include "world/Chunk.hpp"
#include "world/Block.hpp"

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
concept NeighborQuery = 
	std::invocable<Fnc, int, int, int, Direction> &&
	std::convertible_to<std::invoke_result_t<Fnc, int, int, int, Direction>, Block>;

class MeshBuilder
{
private:

public:
	static Mesh BuildMeshNaive(const Chunk& chunk, NeighborQuery auto&& neighbor_query);
	
	static Mesh BuildMeshGreedy(const Chunk& chunk, NeighborQuery auto&& neighbor_query);

private:
	static void SaveQuadMesh(const Chunk& chunk, int x, int y, int z, Direction dir, Mesh& chunk_mesh);

	static uint8_t GetQuadMaterial(BlockType block_type, Direction dir);
};

Mesh MeshBuilder::BuildMeshNaive(const Chunk& chunk, NeighborQuery auto&& neighbor_query)
{
	Mesh chunk_mesh;

	for (int z = 0; z < Constants::Chunk::depth; ++z)
	{
		for (int y = 0; y < Constants::Chunk::height; ++y)
		{
			for (int x = 0; x < Constants::Chunk::width; ++x)
			{
				if (!chunk.BlockAt(x, y, z).IsSolid())
				{
					continue;
				}

				for (Direction dir : AllDirections())
				{
					if (neighbor_query(x, y, z, dir).IsSolid())
					{
						continue;
					}

					SaveQuadMesh(chunk, x, y, z, dir, chunk_mesh);
				}
			}
		}
	}

	return chunk_mesh;
}

Mesh MeshBuilder::BuildMeshGreedy(const Chunk& chunk, NeighborQuery auto&& neighbor_query)
{
	Mesh chunk_mesh;
	std::vector<MaskCell> mask(Constants::chunk::height * Constants::chunk::depth);

	for (int x_boundary = -1; x_boundary < Constants::chunk::width; ++x_boundary)
	{
		//  | | | | | | |
		// -|0|1|2|3|4|5|-
		// -|0|1|2|3|4|5|-
		// -|0|1|2|3|4|5|-
		// -|0|1|2|3|4|5|-
		//  | | | | | | |

		for (int y = 0; y < Constants::chunk::height; ++y)
		{
			for (int z = 0; z < Constants::chunk::depth; ++z)
			{
				// if x == width - 1 ===> needs to handle separately??? ugly if
				const Block& chunk_block = chunk.BlockAt(x_boundary + 1, y, z);
				const Block& neg_x_neighbor = neighbor_query(chunk, x_boundary + 1, y, z, Direction::NegX);

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
	mask.resize(Constants::chunk::depth * Constants::chunk::width);

	for (int y_boundary = 0; y_boundary < Constants::chunk::height + 1; ++y_boundary)
	{
		for (int z = 0; z < Constants::chunk::depth; ++z)
		{
			for (int x = 0; x < Constants::chunk::width; ++x)
			{

			}
		}
	}

	mask.clear();
	mask.resize(Constants::chunk::height * Constants::chunk::width);

	for (int z_boundary = 0; z_boundary < Constants::chunk::depth + 1; ++z_boundary)
	{
		for (int y = 0; y < Constants::chunk::height; ++y)
		{
			for (int x = 0; x < Constants::chunk::width; ++x)
			{

			}
		}
	}

	return chunk_mesh;
}

#endif // MESH_BUILDER_HPP