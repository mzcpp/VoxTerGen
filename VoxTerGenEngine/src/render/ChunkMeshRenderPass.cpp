#include "render/ChunkMeshRenderPass.hpp"
#include "world/World.hpp"
#include "core/ResourceManager.hpp"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ranges>

void ChunkMeshRenderPass::Render(const World& world, const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
	UploadChunkRenderData(world);
	RenderChunks(world.ChunkManagerRef().Chunks(), view, projection, resource_manager);
}

void ChunkMeshRenderPass::UploadChunkRenderData(const World& world)
{
	for (const auto& [world_coords, chunk] : world.ChunkManagerRef().Chunks())
	{
		if (!chunk->MeshNeedsUpload())
		{
			continue;
		}

		auto& render_data = chunk_render_data_[world_coords];
		render_data.gpu_mesh_.UploadMeshData(*chunk->Mesh());
		render_data.chunk_model_ = glm::translate(render_data.chunk_model_, { world_coords.x * constants::chunk::width, 0, world_coords.y * constants::chunk::height });
		chunk->SetMeshNeedsUpload(false);
	}
}

void ChunkMeshRenderPass::RenderChunks(const std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, ivec2_hash>& chunks, const glm::mat4& view, 
	const glm::mat4& projection, const ResourceManager& resource_manager)
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

	for (const auto& chunk : chunks | std::views::values)
	{
		if (chunk == nullptr)
		{
			Logger::Log(LogLevel::CRITICAL, "Unable to render chunk! Chunk is nullptr! Aborting...");
			std::abort();
		}

		const auto& chunk_data_it = chunk_render_data_.find(chunk->WorldCoords());

		if (chunk_data_it == chunk_render_data_.end())
		{
			continue;
		}

		shader_program->Set<glm::mat4>("model", chunk_data_it->second.chunk_model_);
		mesh_renderer_.RenderChunkMesh(chunk_data_it->second.gpu_mesh_);
	}
}