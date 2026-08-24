#ifndef RENDER_DATA_HPP
#define RENDER_DATA_HPP

#include "physics/AABB.hpp"

#include "render/GpuMesh.hpp"

#include <glm/mat4x4.hpp>

struct MeshRenderData
{
    GpuMesh gpu_mesh_;
    glm::mat4 model_matrix_ = glm::mat4(1.0f);
};

struct ChunkRenderData
{
	MeshRenderData mesh_render_data_;
	AABB aabb_;
};

#endif // RENDER_DATA_HPP