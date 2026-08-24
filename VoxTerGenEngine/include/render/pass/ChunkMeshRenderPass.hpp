#ifndef CHUNK_OPAQUE_RENDER_PASS_HPP
#define CHUNK_OPAQUE_RENDER_PASS_HPP

#include "core/ResourceManager.hpp"

#include "physics/AABB.hpp"

#include "render/MeshRenderer.hpp"
#include "render/events/ChunkEvents.hpp"

#include "threading/ThreadSafeQueue.hpp"

class Camera;

struct ChunkRenderData;
struct Plane;

class ChunkMeshRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;

public:
	ChunkMeshRenderPass(const MeshRenderer& mesh_renderer);

	void RenderOpaqueChunks(const std::unordered_map<ChunkID, ChunkRenderData>& chunks_render_data, const std::array<Plane, 6>& frustum_planes, const ResourceManager& resource_manager);
};

#endif // CHUNK_OPAQUE_RENDER_PASS_HPP

