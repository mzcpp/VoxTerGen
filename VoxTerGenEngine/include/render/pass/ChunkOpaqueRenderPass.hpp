#ifndef CHUNK_OPAQUE_RENDER_PASS_HPP
#define CHUNK_OPAQUE_RENDER_PASS_HPP

#include "core/ResourceManager.hpp"

#include "physics/AABB.hpp"

#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"

#include "threading/ThreadSafeQueue.hpp"

#include "world/ChunkEvents.hpp"

#include <unordered_map>

struct ChunkData
{
	MeshRenderData mesh_render_data_;
	AABB aabb_;
};

class Camera;

class ChunkOpaqueRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	std::unordered_map<ChunkID, ChunkData> chunks_data_;

public:
	ChunkOpaqueRenderPass(const MeshRenderer& mesh_renderer);

	void ProcessChunkEvents(ThreadSafeQueue<ChunkEvent>& chunk_event_queue);

	void RenderOpaqueChunks(const Camera& camera, const ResourceManager& resource_manager);
};

#endif // CHUNK_OPAQUE_RENDER_PASS_HPP

