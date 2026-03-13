#include "render/RenderMesh.hpp"

#include <glad/glad/glad.h>

RenderMesh::RenderMesh()
{
    glCreateVertexArrays(1, &vao_);
    glCreateBuffers(1, &vbo_);
    glCreateBuffers(1, &ebo_);
}

RenderMesh::~RenderMesh()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}