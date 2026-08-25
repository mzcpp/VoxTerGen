#include "core/ResourceManager.hpp"

#include "graphics/Camera.hpp"

#include "physics/DigitalDifferentialAnalyzer.hpp"

#include "render/pass/BlockHighlightRenderPass.hpp"
#include "render/pass/ChunkMeshRenderPass.hpp"
#include "render/pass/ChunkWireframeRenderPass.hpp"
#include "render/pass/SkyboxRenderPass.hpp"

#include "render/WorldRenderer.hpp"
#include "render/RenderData.hpp"

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

void WorldRenderer::ProcessChunkEvents(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	while (!chunk_event_queue.Empty())
	{
		const std::optional<ChunkEvent> chunk_event_opt = chunk_event_queue.TryPop();

		if (!chunk_event_opt.has_value())
		{
			continue;
		}

		std::visit(
			overloaded
			{
				[this](const ChunkMeshReady& e)
				{
					ProcessChunkMeshReady(e);
				},

				[this](const ChunkDestroyed& e)
				{
					ProcessChunkDestroyed(e);
				}
			}, 
			*chunk_event_opt
		);
	}
}

void WorldRenderer::ProcessChunkMeshReady(const ChunkMeshReady& event)
{
	 // TODO: Make chunk_mesh_render_data a member variable and reuse the GPU buffers, not erase and allocate new.
	 MeshRenderData chunk_mesh_render_data;
	 chunk_mesh_render_data.gpu_mesh_.InitializeBuffers();
	 chunk_mesh_render_data.gpu_mesh_.UploadMeshData(*event.cpu_chunk_mesh_);
	 chunk_mesh_render_data.model_matrix_ = glm::translate(glm::mat4(1.0f), { event.world_coords_.x * constants::chunk::width, 0, event.world_coords_.y * constants::chunk::depth });

	 const AABB aabb(
	 	glm::dvec3{ event.world_coords_.x * constants::chunk::width, 0, event.world_coords_.y * constants::chunk::depth },
	 	glm::dvec3{ (event.world_coords_.x + 1) * constants::chunk::width, constants::chunk::height, (event.world_coords_.y + 1) * constants::chunk::depth }
	 );

	 // TODO: This fails if entry with event.chunk_id_ already exists.
	 chunks_render_data_.emplace(event.chunk_id_, ChunkRenderData{ std::move(chunk_mesh_render_data), aabb });
}

void WorldRenderer::ProcessChunkDestroyed(const ChunkDestroyed& event)
{
	chunks_render_data_.erase(event.chunk_id_);
}

void WorldRenderer::Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue, const Camera& camera)
{
	ProcessChunkEvents(chunk_event_queue);

	block_highlight_render_pass_.UpdateBlockHighlightModelMatrix(camera.RaycastResult());
}

void WorldRenderer::RenderWorld(const Camera& camera, float alpha, const ResourceManager& resource_manager)
{
	camera_uniform_buffer_.UpdateCameraData(camera, alpha);

	chunk_mesh_render_pass_.RenderOpaqueChunkMeshes(chunks_render_data_, camera.GetFrustumPlanes(), resource_manager);
	block_highlight_render_pass_.RenderBlockHighlight(resource_manager);
	skybox_render_pass_.RenderSkybox(resource_manager);
	chunk_wireframe_render_pass_.RenderChunkWireframe(chunks_render_data_, resource_manager);
}