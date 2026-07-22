#include "render/ChunkMeshRenderPass.hpp"
#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"

#include "core/ResourceManager.hpp"

#include "world/Chunk.hpp"
#include "world/ChunkEvents.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <memory>
#include <queue>
#include <ranges>
#include <variant>

ChunkMeshRenderPass::ChunkMeshRenderPass(const MeshRenderer& mesh_renderer) : 
	mesh_renderer_(mesh_renderer)
{
}


void ChunkMeshRenderPass::ProcessChunkEvents(std::queue<ChunkEvent>& chunk_event_queue)
{
	while (!chunk_event_queue.empty())
	{
		ChunkEvent chunk_event = std::move(chunk_event_queue.front());
		chunk_event_queue.pop();
		
		// TODO: Reuse the GPU buffers, not erase and allocate anew.
		std::visit(overloaded
			{
				[this](chunk_event::ChunkMeshReady& e)
				{
					e.render_data_.gpu_mesh_.InitializeBuffers();
					e.render_data_.gpu_mesh_.UploadMeshData(*e.cpu_mesh_);
					e.render_data_.model_matrix_ = glm::translate(glm::mat4(1.0f), { e.world_coords_.x * constants::chunk::width, 0, e.world_coords_.y * constants::chunk::depth });
					chunks_data_.emplace(e.chunk_id_, std::move(e.render_data_));
				},

				[this](chunk_event::ChunkDestroyed& e)
				{
					chunks_data_.erase(e.chunk_id_);
				}

			}, 
			chunk_event
		);
	}
}

void ChunkMeshRenderPass::RenderChunks(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
	const ShaderProgram* shader_program = resource_manager.GetShaderProgram("chunk_mesh_shader");

	if (shader_program == nullptr)
    {
        return;
    }
	
	shader_program->Use();
	shader_program->Set<glm::mat4>("view", view);
	shader_program->Set<glm::mat4>("projection", projection);
	shader_program->Set<unsigned int>("atlas_columns", constants::texture::atlas_columns);
	shader_program->Set<unsigned int>("atlas_rows", constants::texture::atlas_rows);

	glActiveTexture(GL_TEXTURE0);
	resource_manager.GetTexture("texture_atlas")->Bind();
	shader_program->Set<int>("atlas_texture", 0);

	auto inside_frustum = [](const MeshRenderData& mesh_render_data) { return true; }; 

	for (const MeshRenderData& chunk_data : chunks_data_ | std::views::values | std::views::filter(inside_frustum))
	{
		shader_program->Set<glm::mat4>("model", chunk_data.model_matrix_);
		mesh_renderer_.RenderGpuMesh(chunk_data.gpu_mesh_);
	}

	glUseProgram(0);
}