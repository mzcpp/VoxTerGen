#include "render/GpuMesh.hpp"
#include "mesh/Vertex.hpp"

#include <glad/glad.h>

#include <span>

GpuMesh::GpuMesh() : vao_(0), vbo_(0), ebo_(0), index_count_(0)
{
    glCreateVertexArrays(1, &vao_);
    glCreateBuffers(1, &vbo_);
    glCreateBuffers(1, &ebo_);

    glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(vao_, ebo_);

    glEnableVertexArrayAttrib(vao_, 0);
    glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position_));
    glVertexArrayAttribBinding(vao_, 0, 0);

    glEnableVertexArrayAttrib(vao_, 1);
    glVertexArrayAttribFormat(vao_, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal_));
    glVertexArrayAttribBinding(vao_, 1, 0);

    glEnableVertexArrayAttrib(vao_, 2);
    glVertexArrayAttribFormat(vao_, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv_));
    glVertexArrayAttribBinding(vao_, 2, 0);

    glEnableVertexArrayAttrib(vao_, 3);
    glVertexArrayAttribIFormat(vao_, 3, 1, GL_UNSIGNED_BYTE, offsetof(Vertex, material_));
    glVertexArrayAttribBinding(vao_, 3, 0);
}

GpuMesh::GpuMesh(GpuMesh&& other) noexcept
{
    if (vao_ != 0)
    {
        glDeleteVertexArrays(1, &vao_);
    }

    vao_ = std::exchange(other.vao_, 0);

    if (vbo_ != 0)
    {
        glDeleteBuffers(1, &vbo_);
    }

    vbo_ = std::exchange(other.vbo_, 0);

    if (ebo_ != 0)
    {
        glDeleteBuffers(1, &ebo_);
    }

    ebo_ = std::exchange(other.ebo_, 0);
    index_count_ = std::exchange(other.index_count_, 0);
}

GpuMesh& GpuMesh::operator=(GpuMesh&& other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    if (vao_ != 0)
    {
        glDeleteVertexArrays(1, &vao_);
    }
    
    vao_ = std::exchange(other.vao_, 0);
    
    if (vbo_ != 0)
    {
        glDeleteBuffers(1, &vbo_);
    }

    vao_ = std::exchange(other.vbo_, 0);

    if (ebo_ != 0)
    {
        glDeleteBuffers(1, &ebo_);
    }

    vao_ = std::exchange(other.ebo_, 0);

    return *this;
}

GpuMesh::~GpuMesh()
{
    if (vao_ != 0)
    {
        glDeleteVertexArrays(1, &vao_);
    }
    if (vbo_ != 0)
    {
        glDeleteBuffers(1, &vbo_);
    }

    if (ebo_ != 0)
    {
        glDeleteBuffers(1, &ebo_);
    }
}

void GpuMesh::UploadMeshData(const Mesh& mesh)
{
    const std::vector<Vertex>& vertices = mesh.Vertices();
    const std::vector<std::uint32_t>& indices = mesh.Indices();

    glNamedBufferData(vbo_, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glNamedBufferData(ebo_, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    index_count_ = static_cast<GLsizei>(indices.size());
}