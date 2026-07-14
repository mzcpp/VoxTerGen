#ifndef BLOCK_HIGHLIGHT_RENDER_PASS_HPP
#define BLOCK_HIGHLIGHT_RENDER_PASS_HPP

#include "render/MeshRenderer.hpp"

#include "world/Chunk.hpp"

#include <glm/vec3.hpp>

#include <memory>

class Mesh;
class ResourceManager;

class BlockHighlightRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	Mesh block_highlight_mesh_;
	RenderData render_data_;

public:
	BlockHighlightRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareBlockRenderData();

	void UpdateBlockHighlightModelMatrix(glm::ivec3 block_world_pos);

	void RenderBlockHighlight(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager);
};

#endif

