#include "render/pass/ChunkMeshRenderPass.hpp"
#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

#include "core/ResourceManager.hpp"

#include "mesh/Mesh.hpp"

#include "graphics/Camera.hpp"

#include "math/Geometry.hpp"

#include "physics/AABB.hpp"

#include "threading/ThreadSafeQueue.hpp"

#include "render/events/ChunkEvents.hpp"

#include "world/Chunk.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <memory>
#include <queue>
#include <ranges>
#include <variant>
#include <optional>

ChunkMeshRenderPass::ChunkMeshRenderPass(const MeshRenderer& mesh_renderer) : 
	mesh_renderer_(mesh_renderer)
{
}

void ChunkMeshRenderPass::RenderOpaqueChunks(const std::unordered_map<ChunkID, ChunkRenderData>& chunks_render_data, const std::array<Plane, 6>& frustum_planes, const ResourceManager& resource_manager)
{
	const ShaderProgram* shader_program = resource_manager.GetShaderProgram("chunk_mesh_shader");

	if (shader_program == nullptr)
    {
        return;
    }
	
	shader_program->Use();

	glActiveTexture(GL_TEXTURE0);
	resource_manager.GetTexture("texture_atlas")->Bind();

	const auto inside_frustum = [&frustum_planes](const ChunkRenderData& chunk_data)
	{
		return geometry::Intersects(frustum_planes, chunk_data.aabb_);
	};

	for (const ChunkRenderData& chunk_data : chunks_render_data | std::views::values | std::views::filter(inside_frustum))
	{
		shader_program->Set<glm::mat4>("model", chunk_data.mesh_render_data_.model_matrix_);
		mesh_renderer_.RenderGpuMesh(chunk_data.mesh_render_data_.gpu_mesh_);
	}

	glUseProgram(0);
}