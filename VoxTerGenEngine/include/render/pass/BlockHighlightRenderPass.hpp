#ifndef BLOCK_HIGHLIGHT_RENDER_PASS_HPP
#define BLOCK_HIGHLIGHT_RENDER_PASS_HPP

#include "mesh/Mesh.hpp"

#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

#include <optional>

class ResourceManager;

struct RaycastResult;

class BlockHighlightRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	Mesh block_highlight_mesh_;
	MeshRenderData render_data_;
	bool render_highlight_;

public:
	BlockHighlightRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareBlockRenderData();

	void UpdateBlockHighlightModelMatrix(const std::optional<RaycastResult>& raycast_result);

	void RenderBlockHighlight(const ResourceManager& resource_manager);
};

#endif

