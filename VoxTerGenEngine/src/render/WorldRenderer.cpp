#include "render/WorldRenderer.hpp"
#include "core/ResourceManager.hpp"
#include "graphics/ShaderProgram.hpp"
#include "world/Chunk.hpp"
#include "world/ChunkManager.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

WorldRenderer::WorldRenderer()
{

}

WorldRenderer::~WorldRenderer()
{

}

void WorldRenderer::Initialize()
{

}

void WorldRenderer::RenderWorld(const World& world, const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
	RenderChunks(world.ChunkManager().Chunks(), view, projection, resource_manager);
}

void WorldRenderer::RenderChunks(
	const std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, ivec2_hash>& chunks,
	const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
	const ShaderProgram* shader_program = resource_manager.GetShaderProgram("chunk_mesh_shader");
	
	shader_program->Use();
	shader_program->Set<glm::mat4>("view", view);
	shader_program->Set<glm::mat4>("projection", projection);

	glActiveTexture(GL_TEXTURE0);
	resource_manager.GetTexture("atlas")->Bind();
	shader_program->Set<int>("atlas_texture", 0);
	shader_program->Set<unsigned int>("atlas_columns", constants::texture::atlas_columns);
	shader_program->Set<unsigned int>("atlas_rows", constants::texture::atlas_rows);

	for (const auto& [chunk_coords, chunk] : chunks)
	{
		const glm::mat4 model = glm::translate(glm::mat4(1.0f), { chunk_coords.x * constants::chunk::width, 0, chunk_coords.y * constants::chunk::height });
    	shader_program->Set<glm::mat4>("model", model);

		mesh_renderer_.RenderChunkMesh(chunk_coords, *chunk);
	}
}