#ifndef CHUNK_WIREFRAME_RENDER_PASS_HPP
#define CHUNK_WIREFRAME_RENDER_PASS_HPP

#include "core/ResourceManager.hpp"

#include "physics/AABB.hpp"

#include "mesh/Mesh.hpp"

#include "world/Chunk.hpp"

#include "render/RenderData.hpp"
#include "render/MeshRenderer.hpp"

#include <memory>

class Camera;

struct ChunkRenderData;

class ChunkWireframeRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	Mesh chunk_wireframe_mesh_;
	MeshRenderData chunk_wireframe_mesh_render_data_;
	bool render_wireframe_;

public:
	ChunkWireframeRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareChunkWireframeRenderData();

	void RenderChunkWireframe(const Camera& camera, const ResourceManager& resource_manager);
};

#endif // CHUNK_WIREFRAME_RENDER_PASS_HPP

