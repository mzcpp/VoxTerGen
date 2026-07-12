#ifndef MESH_RENDERER_HPP
#define MESH_RENDERER_HPP

#include "render/GpuMesh.hpp"

#include <glm/vec2.hpp>

class Chunk;

class MeshRenderer
{
private:

public:
	MeshRenderer();

	void RenderGpuMesh(const GpuMesh& gpu_mesh) const;
};

#endif // MESH_RENDERER_HPP
