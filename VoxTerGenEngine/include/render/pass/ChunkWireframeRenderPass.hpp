#ifndef CHUNK_WIREFRAME_RENDER_PASS_HPP
#define CHUNK_WIREFRAME_RENDER_PASS_HPP

#include "core/ResourceManager.hpp"

#include "physics/AABB.hpp"

#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"

#include <memory>

class Mesh;

class ChunkWireframeRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	const std::unique_ptr<Mesh> chunk_wireframe_mesh_;
	bool render_wireframe_;

public:
	ChunkWireframeRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareChunkWireframeRenderData();

	void RenderChunkWireframe(const ResourceManager& resource_manager);
};

#endif // CHUNK_WIREFRAME_RENDER_PASS_HPP

