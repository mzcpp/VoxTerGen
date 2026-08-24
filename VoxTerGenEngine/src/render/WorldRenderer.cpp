#include "core/ResourceManager.hpp"

#include "graphics/Camera.hpp"

#include "physics/DigitalDifferentialAnalyzer.hpp"

#include "render/pass/BlockHighlightRenderPass.hpp"
#include "render/pass/ChunkMeshRenderPass.hpp"
#include "render/pass/ChunkWireframeRenderPass.hpp"
#include "render/pass/SkyboxRenderPass.hpp"

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

	SubscribeToEvents();
}

void WorldRenderer::SubscribeToEvents()
{
	chunk_event_dispatcher_.SubscribeEvent<ChunkMeshReady>(
		[this](const ChunkMeshReady& event)
		{
			chunk_mesh_render_pass_.ProcessChunkMeshReady(event);
			// ProcessChunkMeshReady(event);
		}
	);

	chunk_event_dispatcher_.SubscribeEvent<ChunkDestroyed>(
		[this](const ChunkDestroyed& event)
		{
			chunk_mesh_render_pass_.ProcessChunkDestroyed(event);
			// ProcessChunkDestroyed(event);
		}
	);
}

void WorldRenderer::ProcessChunkMeshReady(const ChunkMeshReady& event)
{
	// // TODO: Make chunk_mesh_render_data a member variable and reuse the GPU buffers, not erase and allocate new.
	// MeshRenderData chunk_mesh_render_data;
	// chunk_mesh_render_data.gpu_mesh_.InitializeBuffers();
	// chunk_mesh_render_data.gpu_mesh_.UploadMeshData(*event.cpu_chunk_mesh_);
	// chunk_mesh_render_data.model_matrix_ = glm::translate(glm::mat4(1.0f), { event.world_coords_.x * constants::chunk::width, 0, event.world_coords_.y * constants::chunk::depth });

	// const AABB aabb(
	// 	glm::dvec3{ event.world_coords_.x * constants::chunk::width, 0, event.world_coords_.y * constants::chunk::depth },
	// 	glm::dvec3{ (event.world_coords_.x + 1) * constants::chunk::width, constants::chunk::height, (event.world_coords_.y + 1) * constants::chunk::depth }
	// );

	// // TODO: This fails if entry with event.chunk_id_ already exists.
	// chunks_data_.emplace(event.chunk_id_, ChunkData{ std::move(chunk_mesh_render_data), aabb });
}

void WorldRenderer::ProcessChunkDestroyed(const ChunkDestroyed& event)
{
	// chunks_data_.erase(event.chunk_id_);
}

void WorldRenderer::Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue, const Camera& camera)
{
	chunk_event_dispatcher_.DispatchEvents(chunk_event_queue);

	block_highlight_render_pass_.UpdateBlockHighlightModelMatrix(camera.RaycastResult());
}

void WorldRenderer::RenderWorld(const Camera& camera, float alpha, const ResourceManager& resource_manager)
{
	camera_uniform_buffer_.UpdateCameraData(camera, alpha);

	chunk_mesh_render_pass_.RenderOpaqueChunks(camera, resource_manager);
	block_highlight_render_pass_.RenderBlockHighlight(resource_manager);
	skybox_render_pass_.RenderSkybox(resource_manager);
	chunk_wireframe_render_pass_.RenderChunkWireframe(resource_manager);
}