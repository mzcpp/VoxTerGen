#include "render/WorldRenderer.hpp"
#include "render/ChunkMeshRenderPass.hpp"
#include "core/ResourceManager.hpp"
#include "graphics/ShaderProgram.hpp"
#include "world/Chunk.hpp"
#include "world/World.hpp"
#include "world/ChunkManager.hpp"

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include <queue>

WorldRenderer::WorldRenderer()
{

}

WorldRenderer::~WorldRenderer()
{

}

void WorldRenderer::RenderWorld(std::queue<ChunkEvent>& chunk_event_queue, const World& world, const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
	chunk_mesh_render_pass_.Render(chunk_event_queue, world, view, projection, resource_manager);
}