#ifndef MESH_RENDER_DATA_HPP
#define MESH_RENDER_DATA_HPP

#include "render/GpuMesh.hpp"

#include <glm/mat4x4.hpp>

struct MeshRenderData
{
    GpuMesh gpu_mesh_;
    glm::mat4 model_matrix_ = glm::mat4(1.0f);
};

#endif