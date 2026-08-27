#ifndef CHUNK_OPAQUE_RENDER_PASS_HPP
#define CHUNK_OPAQUE_RENDER_PASS_HPP

#include "render/MeshRenderer.hpp"

#include "world/Chunk.hpp"

#include <unordered_map>

class ResourceManager;

struct ChunkRenderData;

class ChunkOpaqueRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;

public:
	ChunkOpaqueRenderPass(const MeshRenderer& mesh_renderer);

	void RenderOpaqueChunkMeshes(const std::unordered_map<ChunkID, ChunkRenderData>& chunks_render_data, const ResourceManager& resource_manager);
};

#endif // CHUNK_OPAQUE_RENDER_PASS_HPP