#include "render/pass/ChunkOpaqueRenderPass.hpp"

#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

#include "core/ResourceManager.hpp"

#include "mesh/Mesh.hpp"

#include "world/Chunk.hpp"

#include <glm/mat4x4.hpp>

#include <memory>
#include <ranges>
#include <unordered_map>

ChunkOpaqueRenderPass::ChunkOpaqueRenderPass(const MeshRenderer& mesh_renderer) : 
	mesh_renderer_(mesh_renderer)
{
}

void ChunkOpaqueRenderPass::RenderOpaqueChunkMeshes(const std::unordered_map<ChunkID, ChunkRenderData>& chunks_render_data, const ResourceManager& resource_manager)
{
	const ShaderProgram* shader_program = resource_manager.GetShaderProgram("chunk_mesh_shader");

	if (shader_program == nullptr)
    {
        return;
    }
	
	shader_program->Use();

	glActiveTexture(GL_TEXTURE0);
	resource_manager.GetTexture("texture_atlas")->Bind();

	const auto chunk_visible = [](const ChunkRenderData& chunk_data)
	{
		return chunk_data.visible_;
	};

	for (const ChunkRenderData& chunk_data : chunks_render_data | std::views::values | std::views::filter(chunk_visible))
	{
		shader_program->Set<glm::mat4>("model", chunk_data.mesh_render_data_.model_matrix_);
		mesh_renderer_.RenderGpuMesh(chunk_data.mesh_render_data_.gpu_mesh_);
	}

	glUseProgram(0);
}