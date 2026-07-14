#ifndef GPU_MESH_HPP
#define GPU_MESH_HPP

#include "mesh/Mesh.hpp"

#include <glad/glad/glad.h>

class GpuMesh
{
private:
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLuint ebo_ = 0;
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
    GLuint VAO() const noexcept { return vao_; }
    GLuint VBO() const noexcept { return vbo_; }
    GLuint EBO() const noexcept { return ebo_; }
    GLsizei IndexCount() const noexcept { return index_count_; }
};

#endif // GPU_MESH_HPP

