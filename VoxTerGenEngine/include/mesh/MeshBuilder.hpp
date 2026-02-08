#ifndef MESH_BUILDER_HPP
#define MESH_BUILDER_HPP

#include "mesh/Mesh.hpp"
#include "world/Chunk.hpp"
#include "world/Block.hpp"

#include <functional>

class MeshBuilder
{
private:
public:
	static Mesh BuildMeshNaive(const Chunk& chunk, std::function<Block(int, int, int)> neighbor_query);

	static Mesh BuildMeshGreedy(const Chunk& chunk, std::function<Block(int, int, int)> neighbor_query);
};

#endif // MESH_BUILDER_HPP

