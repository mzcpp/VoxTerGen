#ifndef MESH_RENDERER_HPP
#define MESH_RENDERER_HPP

#include "render/GpuMesh2D.hpp"
#include "render/GpuMesh3D.hpp"

#include <glm/vec2.hpp>

#include <glad/glad.h>

#include <concepts>

class Chunk;

template <typename T>
concept GpuMesh = std::same_as<T, GpuMesh2D> || std::same_as<T, GpuMesh3D>;

class MeshRenderer
{
private:

public:
	template <GpuMesh T>
	void RenderGpuMesh(const T& gpu_mesh) const noexcept
	{
		gpu_mesh.VAO().Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(gpu_mesh.IndexCount()), GL_UNSIGNED_INT, 0);
	}
};

#endif // MESH_RENDERER_HPP
