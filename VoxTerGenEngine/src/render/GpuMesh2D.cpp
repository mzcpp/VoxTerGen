#include "render/GpuMesh2D.hpp"

#include "mesh/Mesh2D.hpp"
#include "mesh/Vertex.hpp"

#include <glad/glad.h>

#include <utility>

GpuMesh2D::GpuMesh2D(GpuMesh2D&& other) noexcept
{
    vao_ = std::move(other.vao_);
    vbo_ = std::move(other.vbo_);
    ebo_ = std::move(other.ebo_);
    index_count_ = std::exchange(other.index_count_, 0);
}

GpuMesh2D& GpuMesh2D::operator=(GpuMesh2D&& other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    vao_ = std::move(other.vao_);
    vbo_ = std::move(other.vbo_);
    ebo_ = std::move(other.ebo_);
    index_count_ = std::exchange(other.index_count_, 0);

    return *this;
}

void GpuMesh2D::InitializeBuffers() noexcept
{
    vao_.Initialize();
    vbo_.Initialize();
    ebo_.Initialize();

    vao_.BindVertexBuffer(0, vbo_, 0, sizeof(Vertex2D));
    vao_.BindElementBuffer(ebo_);

    // Position
    vao_.EnableAttribute(0);
    vao_.SetAttribute(0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, position_));
    vao_.BindAttribute(0, 0);

    // UV
    vao_.EnableAttribute(1);
    vao_.SetAttribute(1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, uv_));
    vao_.BindAttribute(1, 0);
}

void GpuMesh2D::UploadMeshData(const Mesh2D& mesh) noexcept
{
    const std::vector<Vertex2D>& vertices = mesh.Vertices();
    const std::vector<std::uint32_t>& indices = mesh.Indices();

    vbo_.UploadData(vertices.size() * sizeof(Vertex2D), vertices.data(), GL_STATIC_DRAW);
    ebo_.UploadData(indices.size() * sizeof(std::uint32_t), indices.data(), GL_STATIC_DRAW);
    index_count_ = static_cast<GLsizei>(indices.size());
}