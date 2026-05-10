#include "render/WorldRenderer.hpp"

#include "core/ResourceManager.hpp"

#include "world/Chunk.hpp"

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include <queue>

WorldRenderer::WorldRenderer()
{

}

WorldRenderer::~WorldRenderer()
{

}

void WorldRenderer::Tick(std::queue<ChunkEvent>& chunk_event_queue)
{
	chunk_mesh_render_pass_.ProcessChunkEvents(chunk_event_queue);
}


void WorldRenderer::RenderWorld(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
	chunk_mesh_render_pass_.RenderChunks(view, projection, resource_manager);
}