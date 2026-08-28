#ifndef MESH_RENDERER_HPP
#define MESH_RENDERER_HPP

#include "render/GpuMesh3D.hpp"

#include <glm/vec2.hpp>

class Chunk;

class MeshRenderer
{
private:

public:
	void RenderGpuMesh(const GpuMesh3D& gpu_mesh) const noexcept;
};

#endif // MESH_RENDERER_HPP
