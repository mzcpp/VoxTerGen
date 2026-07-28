#ifndef GPU_MESH_HPP
#define GPU_MESH_HPP

#include "graphics/Buffer.hpp"
#include "graphics/VertexArray.hpp"

#include <glad/glad.h>

class Mesh;

class GpuMesh
{
private:
    VertexArray vao_ = 0;
    Buffer vbo_ = 0;
    Buffer ebo_ = 0;
    GLsizei index_count_ = 0;

public:
    GpuMesh() = default;

    GpuMesh(const GpuMesh& other) = delete;
    GpuMesh& operator=(const GpuMesh& other) = delete;

    GpuMesh(GpuMesh&& other) noexcept;
    GpuMesh& operator=(GpuMesh&& other) noexcept;

    ~GpuMesh();

    void InitializeBuffers();

    void UploadMeshData(const Mesh& mesh);

    // Getters
    const VertexArray& VAO() const noexcept { return vao_; }
    const Buffer& VBO() const noexcept { return vbo_; }
    const Buffer& EBO() const noexcept { return ebo_; }
    GLsizei IndexCount() const noexcept { return index_count_; }

private:
    void ReleaseBuffers() noexcept;
};

#endif // GPU_MESH_HPP

