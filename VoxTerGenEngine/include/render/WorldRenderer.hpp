#ifndef WORLD_RENDERER_HPP
#define WORLD_RENDERER_HPP

#include "render/ChunkMeshRenderPass.hpp"
#include "render/BlockHighlightRenderPass.hpp"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <unordered_map>
#include <memory>
#include <queue>
#include <optional>

class ResourceManager;
struct RaycastResult;

class WorldRenderer
{
private:
	MeshRenderer mesh_renderer_;
	ChunkMeshRenderPass chunk_mesh_render_pass_;
	BlockHighlightRenderPass block_highlight_render_pass_;

public:
	WorldRenderer();

	~WorldRenderer();

	void Tick(std::queue<ChunkEvent>& chunk_event_queue, const std::optional<RaycastResult>& raycast_result);

	void RenderWorld(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager);

private:
	void RenderHighlightedBlock();
};

#endif // WORLD_RENDERER_HPP

