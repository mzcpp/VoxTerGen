#ifndef GPU_MESH_2D_HPP
#define GPU_MESH_2D_HPP

#include "graphics/Buffer.hpp"
#include "graphics/VertexArray.hpp"

#include <glad/glad.h>

class Mesh2D;

class GpuMesh2D
{
private:
    VertexArray vao_;
    Buffer vbo_;
    Buffer ebo_;
    GLsizei index_count_ = 0;

public:
    GpuMesh2D() = default;

    GpuMesh2D(const GpuMesh2D& other) = delete;
    GpuMesh2D& operator=(const GpuMesh2D& other) = delete;

    GpuMesh2D(GpuMesh2D&& other) noexcept;
    GpuMesh2D& operator=(GpuMesh2D&& other) noexcept;

    ~GpuMesh2D() = default;

    void InitializeBuffers() noexcept;

    void UploadMeshData(const Mesh2D& mesh) noexcept;

    // Getters
    const VertexArray& VAO() const noexcept { return vao_; }
    const Buffer& VBO() const noexcept { return vbo_; }
    const Buffer& EBO() const noexcept { return ebo_; }
    GLsizei IndexCount() const noexcept { return index_count_; }
};

#endif // GPU_MESH_2D_HPP

