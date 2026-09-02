#include "render/GpuMesh3D.hpp"

#include "mesh/Mesh3D.hpp"
#include "mesh/Vertex.hpp"

#include <glad/glad.h>

#include <utility>

GpuMesh3D::GpuMesh3D(GpuMesh3D&& other) noexcept
{
    vao_ = std::move(other.vao_);
    vbo_ = std::move(other.vbo_);
    ebo_ = std::move(other.ebo_);
    index_count_ = std::exchange(other.index_count_, 0);
}

GpuMesh3D& GpuMesh3D::operator=(GpuMesh3D&& other) noexcept
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

void GpuMesh3D::InitializeBuffers() noexcept
{
    vao_.Initialize();
    vbo_.Initialize();
    ebo_.Initialize();

    vao_.BindVertexBuffer(0, vbo_, 0, sizeof(Vertex3D));
    vao_.BindElementBuffer(ebo_);

    // Position
    vao_.EnableAttribute(0);
    vao_.SetAttribute(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, position_));
    vao_.BindAttribute(0, 0);

    // Normal
    vao_.EnableAttribute(1);
    vao_.SetIntAttribute(1, 1, GL_UNSIGNED_BYTE, offsetof(Vertex3D, normal_));
    vao_.BindAttribute(1, 0);

    // UV
    vao_.EnableAttribute(2);
    vao_.SetAttribute(2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, uv_));
    vao_.BindAttribute(2, 0);

    // Material
    vao_.EnableAttribute(3);
    vao_.SetIntAttribute(3, 1, GL_UNSIGNED_BYTE, offsetof(Vertex3D, material_));
    vao_.BindAttribute(3, 0);
}

void GpuMesh3D::UploadMeshData(const Mesh3D& mesh) noexcept
{
    const std::vector<Vertex3D>& vertices = mesh.Vertices();
    const std::vector<std::uint32_t>& indices = mesh.Indices();

    vbo_.UploadData(vertices.size() * sizeof(Vertex3D), vertices.data(), GL_STATIC_DRAW);
    ebo_.UploadData(indices.size() * sizeof(std::uint32_t), indices.data(), GL_STATIC_DRAW);
    index_count_ = static_cast<GLsizei>(indices.size());
}