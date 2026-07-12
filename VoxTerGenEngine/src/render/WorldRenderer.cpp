#include "core/ResourceManager.hpp"

#include "physics/DigitalDifferentialAnalyzer.hpp"

#include "render/WorldRenderer.hpp"

#include "world/Chunk.hpp"

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include <queue>
#include <optional>

WorldRenderer::WorldRenderer() : 
	chunk_mesh_render_pass_(mesh_renderer_), 
	block_highlight_render_pass_(mesh_renderer_)
{
}

WorldRenderer::~WorldRenderer()
{

}

void WorldRenderer::Tick(std::queue<ChunkEvent>& chunk_event_queue)
{
	chunk_mesh_render_pass_.ProcessChunkEvents(chunk_event_queue);
}

void WorldRenderer::RenderWorld(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager, const std::optional<RaycastResult>& raycast_result)
{
	chunk_mesh_render_pass_.RenderChunks(view, projection, resource_manager);
	RenderHighlightedBlock(raycast_result);
}

void WorldRenderer::RenderHighlightedBlock(const std::optional<RaycastResult>& raycast_result)
{
	if (!raycast_result)
	{
		return;
	}


}
