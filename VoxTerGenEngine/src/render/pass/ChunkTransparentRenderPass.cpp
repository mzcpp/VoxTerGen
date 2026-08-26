#include "render/pass/ChunkTransparentRenderPass.hpp"

#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

#include "core/ResourceManager.hpp"

#include "mesh/Mesh.hpp"

#include "world/Chunk.hpp"

#include <glm/mat4x4.hpp>

#include <vector>

ChunkTransparentRenderPass::ChunkTransparentRenderPass(const MeshRenderer& mesh_renderer) : 
	mesh_renderer_(mesh_renderer)
{

}

void ChunkTransparentRenderPass::RenderTransparentChunkMeshes(const std::vector<const ChunkRenderData*>& transparent_chunks_render_data, const ResourceManager& resource_manager)
{
	const ShaderProgram* shader_program = resource_manager.GetShaderProgram("chunk_mesh_shader");

	if (shader_program == nullptr)
	{
		return;
	}

	shader_program->Use();

	glActiveTexture(GL_TEXTURE0);
	resource_manager.GetTexture("texture_atlas")->Bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	for (const ChunkRenderData* transparent_chunk_data : transparent_chunks_render_data)
	{
		shader_program->Set<glm::mat4>("model", transparent_chunk_data->mesh_render_data_.model_matrix_);
		mesh_renderer_.RenderGpuMesh(transparent_chunk_data->mesh_render_data_.gpu_transparent_mesh_);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	glUseProgram(0);
}