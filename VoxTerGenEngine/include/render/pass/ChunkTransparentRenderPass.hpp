#ifndef CHUNK_TRANSPARENT_RENDER_PASS_HPP
#define CHUNK_TRANSPARENT_RENDER_PASS_HPP

#include "core/ResourceManager.hpp"

#include "physics/AABB.hpp"

#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"

class ChunkTransparentRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;

public:
	ChunkTransparentRenderPass(const MeshRenderer& mesh_renderer);

	void RenderTransparentChunks(const ResourceManager& resource_manager);
};

#endif // CHUNK_TRANSPARENT_RENDER_PASS_HPP

