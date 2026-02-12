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
	static Mesh BuildMeshNaive(const Chunk& chunk, std::function<Block(glm::ivec2, int, int, int, Direction)> neighbor_query);

	static Mesh BuildMeshGreedy(const Chunk& chunk, std::function<Block(glm::ivec2, int, int, int, Direction)> neighbor_query);

private:
	static void SaveQuadMesh(const Chunk& chunk, int x, int y, int z, std::function<Block(glm::ivec2, int, int, int, Direction)> neighbor_query, Direction dir, Mesh& chunk_mesh);

	static Material GetQuadMaterial(BlockType block_type, Direction dir);
};

#endif // MESH_BUILDER_HPP

