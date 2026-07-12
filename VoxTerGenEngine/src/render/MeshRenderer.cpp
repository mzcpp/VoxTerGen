#include "render/MeshRenderer.hpp"

#include "world/Chunk.hpp"

#include "utils/Constants.hpp"

#include "render/GpuMesh.hpp"

#include <glad/glad.h>

MeshRenderer::MeshRenderer()
{
}

void MeshRenderer::RenderGpuMesh(const GpuMesh& gpu_mesh) const
{
    glBindVertexArray(gpu_mesh.VAO());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(gpu_mesh.IndexCount()), GL_UNSIGNED_INT, 0);
}
