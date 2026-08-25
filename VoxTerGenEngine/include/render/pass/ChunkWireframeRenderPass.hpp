#ifndef CHUNK_WIREFRAME_RENDER_PASS_HPP
#define CHUNK_WIREFRAME_RENDER_PASS_HPP

#include "core/ResourceManager.hpp"

#include "physics/AABB.hpp"

#include "mesh/Mesh.hpp"

#include "render/MeshRenderer.hpp"

#include <memory>

class Mesh;

class ChunkWireframeRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	Mesh chunk_wireframe_mesh_;
	bool render_wireframe_;

public:
	ChunkWireframeRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareChunkWireframeRenderData();

	void RenderChunkWireframe(const std::unordered_map<ChunkID, ChunkRenderData>& chunks_render_data, const std::array<Plane, 6>& frustum_planes, const ResourceManager& resource_manager);
};

#endif // CHUNK_WIREFRAME_RENDER_PASS_HPP

