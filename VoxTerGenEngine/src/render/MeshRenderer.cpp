#include "render/MeshRenderer.hpp"
#include "world/Chunk.hpp"
#include "utils/Constants.hpp"

#include <glm/vec2.hpp>

#include <glad/glad.h>

MeshRenderer::MeshRenderer()
{
}

void MeshRenderer::RenderChunkMesh(const glm::ivec2& chunk_coords, const Chunk& chunk)
{
    glBindVertexArray(chunk.GpuMesh().VAO());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(chunk.Mesh().Indices().size()), GL_UNSIGNED_INT, 0);
}
