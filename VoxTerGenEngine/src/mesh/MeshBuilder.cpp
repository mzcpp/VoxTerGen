#include "mesh/MeshBuilder.hpp"
#include "world/ChunkManager.hpp"

Mesh MeshBuilder::BuildMeshNaive(const Chunk& chunk, std::function<Block(int, int, int)> neighbor_query)
{
	Mesh result;
	const glm::ivec2& world_coords = chunk.WorldCoords();

	for (const Block& block : chunk.Blocks())
	{
		if (!block.IsSolid())
		{
			continue;
		}

		// in this order
		// +X -X +Y -Y +Z -Z
		// check neighbor on that side
		// if solid - skip this face
		// else, emit this quad - save the vertices and indices to result

	}

	return result;
}

Mesh MeshBuilder::BuildMeshGreedy(const Chunk& chunk, std::function<Block(int, int, int)> neighbor_query)
{
	Mesh result;
	return result;
}