#include "mesh/Vertex.hpp"
#include "mesh/Mesh.hpp"

#include "render/GpuMesh.hpp"

#include <glad/glad.h>

#include <utility>

GpuMesh::GpuMesh(GpuMesh&& other) noexcept
{
    vao_ = std::move(other.vao_);
    vbo_ = std::move(other.vbo_);
    ebo_ = std::move(other.ebo_);
    index_count_ = std::exchange(other.index_count_, 0);
}

GpuMesh& GpuMesh::operator=(GpuMesh&& other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    ReleaseBuffers();

    vao_ = std::move(other.vao_);
    vbo_ = std::move(other.vbo_);
    ebo_ = std::move(other.ebo_);
    index_count_ = std::exchange(other.index_count_, 0);

    return *this;
}

GpuMesh::~GpuMesh()
{
    ReleaseBuffers();
}

void GpuMesh::InitializeBuffers() noexcept
{
    vao_.Initialize();
    vbo_.Initialize();
    ebo_.Initialize();

    vao_.BindVertexBuffer(0, vbo_, 0, sizeof(Vertex));
    vao_.BindElementBuffer(ebo_);

    // Position
    vao_.EnableAttribute(0);
    vao_.SetAttribute(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position_));
    vao_.BindAttribute(0, 0);

    // Normal
    vao_.EnableAttribute(1);
    vao_.SetAttribute(1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal_));
    vao_.BindAttribute(1, 0);

    // UV
    vao_.EnableAttribute(2);
    vao_.SetAttribute(2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv_));
    vao_.BindAttribute(2, 0);

    // Material
    vao_.EnableAttribute(3);
    vao_.SetAttribute(3, 1, GL_UNSIGNED_BYTE, offsetof(Vertex, material_));
    vao_.BindAttribute(3, 0);
}

void GpuMesh::ReleaseBuffers() noexcept
{
    vao_.Release();
    vbo_.Release();
    ebo_.Release();
}

void GpuMesh::UploadMeshData(const Mesh& mesh) noexcept
{
    const std::vector<Vertex>& vertices = mesh.Vertices();
    const std::vector<std::uint32_t>& indices = mesh.Indices();

    vbo_.UploadData(vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    ebo_.UploadData(indices.size() * sizeof(std::uint32_t), indices.data(), GL_STATIC_DRAW);
    index_count_ = static_cast<GLsizei>(indices.size());
}