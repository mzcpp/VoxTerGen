#ifndef BLOCK_HIGHLIGHT_RENDER_PASS_HPP
#define BLOCK_HIGHLIGHT_RENDER_PASS_HPP

#include "render/MeshRenderer.hpp"

#include "world/Chunk.hpp"

#include <memory>

class Mesh;

class BlockHighlightRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	Mesh block_highlight_mesh_;
	RenderData render_data_;

public:
	BlockHighlightRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareBlockRenderData();

	void RenderBlockHighlight(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager);
};

#endif

