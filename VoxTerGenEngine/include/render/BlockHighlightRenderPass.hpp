#ifndef BLOCK_HIGHLIGHT_RENDER_PASS_HPP
#define BLOCK_HIGHLIGHT_RENDER_PASS_HPP

#include "render/MeshRenderer.hpp"

class BlockHighlightRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;

public:
	BlockHighlightRenderPass(const MeshRenderer& mesh_renderer);

	void RenderBlockHighlight();
};

#endif

