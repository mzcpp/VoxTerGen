#ifndef MESH_BUILDER_HPP
#define MESH_BUILDER_HPP

#include "mesh/Mesh.hpp"
#include "world/Chunk.hpp"
#include "world/Block.hpp"

#include <concepts>

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
	return {};
}

#endif // MESH_BUILDER_HPP