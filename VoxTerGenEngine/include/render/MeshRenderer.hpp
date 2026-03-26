#ifndef MESH_RENDERER_HPP
#define MESH_RENDERER_HPP

#include <glm/vec2.hpp>

class Chunk;

class MeshRenderer
{
private:

public:
	MeshRenderer();

	void RenderChunkMesh(const glm::ivec2& chunk_coords, const Chunk& chunk);
};

#endif // MESH_RENDERER_HPP
