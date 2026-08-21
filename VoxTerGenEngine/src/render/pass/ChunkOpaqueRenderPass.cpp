#include "render/pass/ChunkOpaqueRenderPass.hpp"
#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"

#include "core/ResourceManager.hpp"

#include "mesh/Mesh.hpp"

#include "graphics/Camera.hpp"

#include "math/Geometry.hpp"

#include "physics/AABB.hpp"

#include "threading/ThreadSafeQueue.hpp"

#include "render/events/ChunkEvents.hpp"

#include "world/Chunk.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <memory>
#include <queue>
#include <ranges>
#include <variant>
#include <optional>

ChunkOpaqueRenderPass::ChunkOpaqueRenderPass(const MeshRenderer& mesh_renderer) : 
	mesh_renderer_(mesh_renderer)
{
}

void ChunkOpaqueRenderPass::ProcessChunkEvents(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	while (!chunk_event_queue.Empty())
	{
		std::optional<ChunkEvent> chunk_event = chunk_event_queue.TryPop();

		if (!chunk_event.has_value())
		{
			continue;
		}
		
		std::visit(overloaded
			{
				[this](chunk_event::ChunkMeshReady& e)
				{
					// TODO: Make render_data a member variable and reuse the GPU buffers, not erase and allocate new.
					MeshRenderData render_data;
					render_data.gpu_mesh_.InitializeBuffers();
					render_data.gpu_mesh_.UploadMeshData(*e.cpu_mesh_);
					render_data.model_matrix_ = glm::translate(glm::mat4(1.0f), { e.world_coords_.x * constants::chunk::width, 0, e.world_coords_.y * constants::chunk::depth });

					AABB aabb(
						glm::dvec3{ e.world_coords_.x * constants::chunk::width, 0, e.world_coords_.y * constants::chunk::depth }, 
						glm::dvec3{ (e.world_coords_.x + 1) * constants::chunk::width, constants::chunk::height, (e.world_coords_.y + 1) * constants::chunk::depth }
					);

					chunks_data_.emplace(e.chunk_id_, ChunkData{ std::move(render_data), aabb });
				},

				[this](chunk_event::ChunkDestroyed& e)
				{
					chunks_data_.erase(e.chunk_id_);
				}
			}, 
			*chunk_event
		);
	}
}

void ChunkOpaqueRenderPass::RenderOpaqueChunks(const Camera& camera, const ResourceManager& resource_manager)
{
	const ShaderProgram* shader_program = resource_manager.GetShaderProgram("chunk_mesh_shader");

	if (shader_program == nullptr)
    {
        return;
    }
	
	shader_program->Use();

	glActiveTexture(GL_TEXTURE0);
	resource_manager.GetTexture("texture_atlas")->Bind();

	auto& frustum = camera.GetFrustumPlanes();

	const auto inside_frustum = [frustum = std::move(frustum)](const ChunkData& chunk_data) {
		return geometry::Intersects(frustum, chunk_data.aabb_);
	};

	for (const ChunkData& chunk_data : chunks_data_ | std::views::values | std::views::filter(inside_frustum))
	{
		shader_program->Set<glm::mat4>("model", chunk_data.mesh_render_data_.model_matrix_);
		mesh_renderer_.RenderGpuMesh(chunk_data.mesh_render_data_.gpu_mesh_);
	}

	glUseProgram(0);
}