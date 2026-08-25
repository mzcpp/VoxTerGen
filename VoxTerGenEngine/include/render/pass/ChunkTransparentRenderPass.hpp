#ifndef CHUNK_TRANSPARENT_RENDER_PASS_HPP
#define CHUNK_TRANSPARENT_RENDER_PASS_HPP

#include "core/ResourceManager.hpp"

#include "render/MeshRenderer.hpp"

#include "world/Chunk.hpp"

#include <unordered_map>

struct ChunkRenderData;

class ChunkTransparentRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;

public:
	ChunkTransparentRenderPass(const MeshRenderer& mesh_renderer);

	void RenderTransparentChunkMeshes(const std::unordered_map<ChunkID, ChunkRenderData>& chunks_render_data, const ResourceManager& resource_manager);
};

#endif // CHUNK_TRANSPARENT_RENDER_PASS_HPP

