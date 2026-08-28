#include "render/MeshRenderer.hpp"

#include "render/GpuMesh3D.hpp"

#include <glad/glad.h>

void MeshRenderer::RenderGpuMesh(const GpuMesh3D& gpu_mesh) const noexcept
{
    gpu_mesh.VAO().Bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(gpu_mesh.IndexCount()), GL_UNSIGNED_INT, 0);
}
