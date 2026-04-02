#include "render/ChunkMeshRenderPass.hpp"
#include "world/World.hpp"
#include "core/ResourceManager.hpp"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ranges>
#include <iostream>

void ChunkMeshRenderPass::Render(std::queue<ChunkEvent>& chunk_event_queue, const World& world, const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
	// TODO: call ProcessChunkEvents here or in WorldRenderer::RenderWorld? (one level higher)
	ProcessChunkEvents(chunk_event_queue);
	RenderChunks(view, projection, resource_manager);
}

void ChunkMeshRenderPass::ProcessChunkEvents(std::queue<ChunkEvent>& chunk_event_queue)
{
	while (!chunk_event_queue.empty())
	{
		const ChunkEvent& chunk_event = chunk_event_queue.front();
		chunk_event_queue.pop();
		
		// std::visit here?
		// TODO: Reuse the GPU buffers, not erase and allocate anew.
		if (std::holds_alternative<ChunkMeshReady>(chunk_event))
		{
			const ChunkMeshReady& event = std::get<ChunkMeshReady>(chunk_event);

			ChunkRenderData render_data;
			render_data.gpu_mesh_.UploadMeshData(event.cpu_mesh_.get());
			render_data.chunk_model_ = glm::translate(glm::mat4(1.0f), { event.world_coords_.x * constants::chunk::width, 0, event.world_coords_.y * constants::chunk::depth });
			// TODO: Check if it was emplaced??
			chunks_render_data_.try_emplace(event.chunk_id_, render_data);
		}
		else if (std::holds_alternative<ChunkDestroyed>(chunk_event))
		{
			const ChunkDestroyed& event = std::get<ChunkDestroyed>(chunk_event);
			chunks_render_data_.erase(event.chunk_id_);
		}
	}
}

void ChunkMeshRenderPass::RenderChunks(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
	const ShaderProgram* shader_program = resource_manager.GetShaderProgram("chunk_mesh_shader");

	shader_program->Use();
	shader_program->Set<glm::mat4>("view", view);
	shader_program->Set<glm::mat4>("projection", projection);
	shader_program->Set<unsigned int>("atlas_columns", constants::texture::atlas_columns);
	shader_program->Set<unsigned int>("atlas_rows", constants::texture::atlas_rows);

	glActiveTexture(GL_TEXTURE0);
	resource_manager.GetTexture("atlas")->Bind();
	shader_program->Set<int>("atlas_texture", 0);

	for (const ChunkRenderData& chunk_render_data : chunks_render_data_ | std::views::values)
	{
		shader_program->Set<glm::mat4>("model", chunk_render_data.chunk_model_);
		mesh_renderer_.RenderChunkMesh(chunk_render_data.gpu_mesh_);
	}
}