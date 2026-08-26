#ifndef RENDER_DATA_HPP
#define RENDER_DATA_HPP

#include "physics/AABB.hpp"

#include "render/GpuMesh.hpp"

#include <glm/mat4x4.hpp>

struct MeshRenderData
{
    GpuMesh gpu_opaque_mesh_;
    GpuMesh gpu_transparent_mesh_;
    glm::mat4 model_matrix_ = glm::mat4(1.0f);
};

struct ChunkRenderData
{
	MeshRenderData mesh_render_data_;
	AABB aabb_;
    bool visible_ = false;
};

#endif // RENDER_DATA_HPP