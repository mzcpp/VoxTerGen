#ifndef CHUNK_TRANSPARENT_RENDER_PASS_HPP
#define CHUNK_TRANSPARENT_RENDER_PASS_HPP

#include "render/MeshRenderer.hpp"

#include <vector>

class ResourceManager;

struct TransparentChunkData;

class ChunkTransparentRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;

public:
	ChunkTransparentRenderPass(const MeshRenderer& mesh_renderer);

	void RenderTransparentChunkMeshes(const std::vector<TransparentChunkData>& transparent_chunks_data, const ResourceManager& resource_manager);
};

#endif // CHUNK_TRANSPARENT_RENDER_PASS_HPP

