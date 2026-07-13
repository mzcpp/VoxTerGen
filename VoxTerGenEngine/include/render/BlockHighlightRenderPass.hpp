#ifndef BLOCK_HIGHLIGHT_RENDER_PASS_HPP
#define BLOCK_HIGHLIGHT_RENDER_PASS_HPP

#include "render/MeshRenderer.hpp"

#include "world/Chunk.hpp"

#include "mesh/Mesh.hpp"

#include <memory>

class BlockHighlightRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	// CPU MESH ONCE GENERATED
    ChunkRenderData render_data_;

public:
	BlockHighlightRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareBlockRenderData(const Mesh& mesh);

	void RenderBlockHighlight();
};

#endif

