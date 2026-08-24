#ifndef CHUNK_OPAQUE_RENDER_PASS_HPP
#define CHUNK_OPAQUE_RENDER_PASS_HPP

#include "core/ResourceManager.hpp"

#include "physics/AABB.hpp"

#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"
#include "render/events/ChunkEvents.hpp"

#include "threading/ThreadSafeQueue.hpp"


#include <unordered_map>

struct ChunkData
{
	MeshRenderData mesh_render_data_;
	AABB aabb_;
};

class Camera;

class ChunkMeshRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	std::unordered_map<ChunkID, ChunkData> chunks_data_;

public:
	ChunkMeshRenderPass(const MeshRenderer& mesh_renderer);

	void ProcessChunkMeshReady(const ChunkMeshReady& event);
	
	void ProcessChunkDestroyed(const ChunkDestroyed& event);

	void RenderOpaqueChunks(const Camera& camera, const ResourceManager& resource_manager);
};

#endif // CHUNK_OPAQUE_RENDER_PASS_HPP

