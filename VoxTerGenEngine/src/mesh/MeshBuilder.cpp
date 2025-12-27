#include "mesh/MeshBuilder.hpp"

Mesh MeshBuilder::BuildMesh(const Chunk& chunk, std::function<Block(int, int, int)> neighbor_query)
{
	Mesh result;

	for (const Block& block : chunk.Data())
	{

	}

	return result;
}