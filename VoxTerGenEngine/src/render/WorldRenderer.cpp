#include "render/WorldRenderer.hpp"
#include "render/ChunkMeshRenderPass.hpp"
#include "core/ResourceManager.hpp"
#include "graphics/ShaderProgram.hpp"
#include "world/Chunk.hpp"
#include "world/World.hpp"
#include "world/ChunkManager.hpp"

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

WorldRenderer::WorldRenderer()
{

}

WorldRenderer::~WorldRenderer()
{

}

void WorldRenderer::InitializeChunkRenderData(const World& world)
{
	chunk_mesh_render_pass_.InitializeChunkRenderData(world);
}

void WorldRenderer::RenderWorld(const World& world, const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
	chunk_mesh_render_pass_.Render(world, view, projection, resource_manager);
}