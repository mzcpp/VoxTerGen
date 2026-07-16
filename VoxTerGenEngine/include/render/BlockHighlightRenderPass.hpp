#ifndef BLOCK_HIGHLIGHT_RENDER_PASS_HPP
#define BLOCK_HIGHLIGHT_RENDER_PASS_HPP

#include "render/MeshRenderer.hpp"

#include "world/Chunk.hpp"

#include <glm/vec3.hpp>

#include <memory>

class Mesh;
class ResourceManager;

struct RaycastResult;

class BlockHighlightRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	Mesh block_highlight_mesh_;
	RenderData render_data_;
	bool render_highlight_;

public:
	BlockHighlightRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareBlockRenderData();

	void UpdateBlockHighlightModelMatrix(const std::optional<RaycastResult>& raycast_result);

	void RenderBlockHighlight(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager);
};

#endif

