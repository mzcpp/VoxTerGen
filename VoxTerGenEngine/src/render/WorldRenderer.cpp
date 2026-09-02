#include "render/WorldRenderer.hpp"

#include "core/Application.hpp"
#include "core/ResourceManager.hpp"

#include "graphics/Camera.hpp"

#include "math/Geometry.hpp"

#include "physics/DigitalDifferentialAnalyzer.hpp"

#include "render/RenderData.hpp"
#include "render/pass/BlockHighlightRenderPass.hpp"
#include "render/pass/ChunkOpaqueRenderPass.hpp"
#include "render/pass/ChunkWireframeRenderPass.hpp"
#include "render/pass/SkyboxRenderPass.hpp"

#include "world/Chunk.hpp"

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include <optional>
#include <ranges>

WorldRenderer::WorldRenderer(const ScreenDimensionsData& screen_dimensions_data, const ResourceManager& resource_manager, const Camera& camera) :
	screen_dimensions_data_(screen_dimensions_data), 
	resource_manager_(resource_manager), 
	camera_(camera), 
	chunk_opaque_render_pass_(mesh_renderer_),
	block_highlight_render_pass_(mesh_renderer_), 
	skybox_render_pass_(mesh_renderer_), 
	chunk_wireframe_render_pass_(mesh_renderer_), 
	chunk_transparent_render_pass_(mesh_renderer_), 
	ui_render_pass_(mesh_renderer_, screen_dimensions_data_)
{
}

void WorldRenderer::Initialize()
{
	camera_uniform_buffer_.Initialize();

	block_highlight_render_pass_.PrepareBlockRenderData();
	skybox_render_pass_.PrepareSkyboxRenderData();
	chunk_wireframe_render_pass_.PrepareChunkWireframeRenderData();
	ui_render_pass_.PrepareCrosshairRenderData(resource_manager_);
}

void WorldRenderer::ProcessChunkEvents(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	while (const std::optional<ChunkEvent> chunk_event_opt = chunk_event_queue.TryPop())
	{
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
	if (event.chunk_->MeshId() != event.mesh_id_)
	{
		return;
	}

	const glm::ivec2 chunk_world_coords = event.chunk_->WorldCoords();

	ChunkMeshRenderData chunk_mesh_render_data;
	chunk_mesh_render_data.gpu_opaque_mesh_.InitializeBuffers();
	chunk_mesh_render_data.gpu_opaque_mesh_.UploadMeshData(event.chunk_mesh_->cpu_opaque_mesh_);
	chunk_mesh_render_data.gpu_transparent_mesh_.InitializeBuffers();
	chunk_mesh_render_data.gpu_transparent_mesh_.UploadMeshData(event.chunk_mesh_->cpu_transparent_mesh_);
	chunk_mesh_render_data.model_matrix_ = glm::translate(glm::mat4(1.0f), { chunk_world_coords.x * constants::chunk::width, 0, chunk_world_coords.y * constants::chunk::depth });

	const AABB chunk_aabb(
		glm::dvec3{ chunk_world_coords.x * constants::chunk::width, 0, chunk_world_coords.y * constants::chunk::depth },
		glm::dvec3{ (chunk_world_coords.x + 1) * constants::chunk::width, constants::chunk::height, (chunk_world_coords.y + 1) * constants::chunk::depth }
	);

	chunks_render_data_.insert_or_assign(event.chunk_->Id(), ChunkRenderData{std::move(chunk_mesh_render_data), chunk_aabb, false});
}

void WorldRenderer::ProcessChunkDestroyed(const ChunkDestroyed& event)
{
	chunks_render_data_.erase(event.chunk_id_);
}

void WorldRenderer::Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	ProcessChunkEvents(chunk_event_queue);
	UpdateChunksVisibility();
	UpdateTransparentChunks();
	
	ui_render_pass_.UpdateProjection();
	block_highlight_render_pass_.UpdateBlockHighlightModelMatrix(camera_.RaycastResult());
}

void WorldRenderer::RenderWorld(float alpha)
{
	camera_uniform_buffer_.UpdateCameraData(camera_, alpha);

	chunk_opaque_render_pass_.RenderOpaqueChunkMeshes(chunks_render_data_, resource_manager_);
	block_highlight_render_pass_.RenderBlockHighlight(resource_manager_);
	skybox_render_pass_.RenderSkybox(resource_manager_);
	chunk_wireframe_render_pass_.RenderChunkWireframe(camera_, resource_manager_);
	chunk_transparent_render_pass_.RenderTransparentChunkMeshes(transparent_chunks_data_, resource_manager_);
	ui_render_pass_.RenderCrosshair(resource_manager_);
}

void WorldRenderer::UpdateChunksVisibility()
{
	for (ChunkRenderData& chunk_render_data : chunks_render_data_ | std::views::values)
	{
		chunk_render_data.visible_ = geometry::Intersects(camera_.GetFrustumPlanes(), chunk_render_data.aabb_);
	}
}

void WorldRenderer::UpdateTransparentChunks()
{
	transparent_chunks_data_.clear();

	for (ChunkRenderData& chunk_render_data : chunks_render_data_ | std::views::values)
	{
		if (chunk_render_data.visible_ && chunk_render_data.mesh_render_data_.gpu_transparent_mesh_.IndexCount() != 0)
		{
			const glm::dvec3 aabb_center = (chunk_render_data.aabb_.min_ + chunk_render_data.aabb_.max_) / 2.0;

			transparent_chunks_data_.emplace_back(TransparentChunkData{ &chunk_render_data, geometry::DistanceSquared(camera_.Pos(), aabb_center) });
		}
	}

	std::ranges::sort(transparent_chunks_data_,
		[this](const TransparentChunkData& lhs, const TransparentChunkData& rhs)
		{
			return lhs.distance_squared_ > rhs.distance_squared_;
		}
	);
}