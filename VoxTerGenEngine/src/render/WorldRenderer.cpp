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

void WorldRenderer::Initialize()
{
	block_highlight_render_pass_.PrepareBlockRenderData();
}

void WorldRenderer::Tick(std::queue<ChunkEvent>& chunk_event_queue, const std::optional<RaycastResult>& raycast_result)
{
	chunk_mesh_render_pass_.ProcessChunkEvents(chunk_event_queue);
	block_highlight_render_pass_.UpdateBlockHighlightModelMatrix(raycast_result);
}

void WorldRenderer::RenderWorld(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
	chunk_mesh_render_pass_.RenderChunks(view, projection, resource_manager);
	block_highlight_render_pass_.RenderBlockHighlight(view, projection, resource_manager);
}