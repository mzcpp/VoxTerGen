#ifndef MESH_RENDERER_HPP
#define MESH_RENDERER_HPP

class Chunk;

class MeshRenderer
{
private:

public:
	MeshRenderer();

	void RenderChunkMesh(const Chunk& chunk);
};

#endif // MESH_RENDERER_HPP
