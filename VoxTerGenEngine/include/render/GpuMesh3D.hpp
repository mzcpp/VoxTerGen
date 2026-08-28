#ifndef GPU_MESH_3D_HPP
#define GPU_MESH_3D_HPP

#include "graphics/Buffer.hpp"
#include "graphics/VertexArray.hpp"

#include <glad/glad.h>

class Mesh3D;

class GpuMesh3D
{
private:
    VertexArray vao_;
    Buffer vbo_;
    Buffer ebo_;
    GLsizei index_count_ = 0;

public:
    GpuMesh3D() = default;

    GpuMesh3D(const GpuMesh3D& other) = delete;
    GpuMesh3D& operator=(const GpuMesh3D& other) = delete;

    GpuMesh3D(GpuMesh3D&& other) noexcept;
    GpuMesh3D& operator=(GpuMesh3D&& other) noexcept;

    ~GpuMesh3D() = default;

    void InitializeBuffers() noexcept;

    void UploadMeshData(const Mesh3D& mesh) noexcept;

    // Getters
    const VertexArray& VAO() const noexcept { return vao_; }
    const Buffer& VBO() const noexcept { return vbo_; }
    const Buffer& EBO() const noexcept { return ebo_; }
    GLsizei IndexCount() const noexcept { return index_count_; }
};

#endif // GPU_MESH_3D_HPP

