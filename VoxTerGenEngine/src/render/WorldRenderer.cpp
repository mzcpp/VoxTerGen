#include "core/ResourceManager.hpp"

#include "graphics/Camera.hpp"

#include "physics/DigitalDifferentialAnalyzer.hpp"

#include "render/BlockHighlightRenderPass.hpp"
#include "render/ChunkMeshRenderPass.hpp"
#include "render/ChunkWireframeRenderPass.hpp"
#include "render/SkyboxRenderPass.hpp"
#include "render/WorldRenderer.hpp"

#include "world/Chunk.hpp"

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include <queue>
#include <optional>

WorldRenderer::WorldRenderer() : 
	chunk_mesh_render_pass_(mesh_renderer_), 
	block_highlight_render_pass_(mesh_renderer_), 
	skybox_render_pass_(mesh_renderer_), 
	chunk_wireframe_render_pass_(mesh_renderer_)
{
}

void WorldRenderer::Initialize()
{
	camera_uniform_buffer_.Initialize();

	block_highlight_render_pass_.PrepareBlockRenderData();
	skybox_render_pass_.PrepareSkyboxRenderData();
	chunk_wireframe_render_pass_.PrepareChunkWireframeRenderData();
}

void WorldRenderer::Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue, const Camera& camera)
{
	chunk_mesh_render_pass_.ProcessChunkEvents(chunk_event_queue);
	block_highlight_render_pass_.UpdateBlockHighlightModelMatrix(camera.RaycastResult());
}

void WorldRenderer::RenderWorld(const Camera& camera, float alpha, const ResourceManager& resource_manager)
{
	camera_uniform_buffer_.UpdateMatrices(camera, alpha);

	chunk_mesh_render_pass_.RenderChunks(camera, resource_manager);
	block_highlight_render_pass_.RenderBlockHighlight(resource_manager);
	skybox_render_pass_.RenderSkybox(resource_manager);
	chunk_wireframe_render_pass_.RenderChunkWireframe(resource_manager);
}