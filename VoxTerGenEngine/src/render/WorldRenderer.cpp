#include "render/WorldRenderer.hpp"
#include "core/ResourceManager.hpp"
#include "graphics/ShaderProgram.hpp"

#include <glad/glad.h>

WorldRenderer::WorldRenderer()
{

}

WorldRenderer::~WorldRenderer()
{

}

void WorldRenderer::Initialize()
{

}

void WorldRenderer::RenderChunks(
	const std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, ivec2_hash>& chunks,
	const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager
	)
{
	const ShaderProgram* shader_program = resource_manager.GetShaderProgram("chunk_mesh_shader");
	
	shader_program->Use();
	shader_program->Set<glm::mat4>("view", view);
	shader_program->Set<glm::mat4>("projection", projection);

	glActiveTexture(GL_TEXTURE0);
	resource_manager.GetTexture("atlas")->Bind();
	shader_program->Set<int>("texture1", 0);

	//glBindVertexArray(chunk.gpu_mesh.vao_)
	
	// loop over visible chunks whose mesh_invalid_ flag is true
	// mesh_renderer_.RenderChunkMesh(chunk, );
}