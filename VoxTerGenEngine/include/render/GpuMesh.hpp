#ifndef GPU_MESH_HPP
#define GPU_MESH_HPP

#include <mesh/Mesh.hpp>

#include <glad/glad/glad.h>

class GpuMesh
{
private:
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
    GLsizei index_count_;

public:
    GpuMesh();

    GpuMesh(const GpuMesh& other) = delete;
    GpuMesh& operator=(const GpuMesh& other) = delete;

    GpuMesh(GpuMesh&& other) noexcept;
    GpuMesh& operator=(GpuMesh&& other) noexcept;

    ~GpuMesh();

    void UploadMeshData(const Mesh& mesh);

    // Getters
    GLuint VAO() const { return vao_; }
    GLuint VBO() const { return vbo_; }
    GLuint EBO() const { return ebo_; }
    GLsizei IndexCount() const { return index_count_; }
};

#endif // GPU_MESH_HPP

