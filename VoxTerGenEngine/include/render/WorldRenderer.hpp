#ifndef WORLD_RENDERER_HPP
#define WORLD_RENDERER_HPP

#include "render/events/ChunkEvents.hpp"
#include "render/events/EventDispatcher.hpp"

#include "render/pass/BlockHighlightRenderPass.hpp"
#include "render/pass/ChunkWireframeRenderPass.hpp"
#include "render/pass/ChunkMeshRenderPass.hpp"
#include "render/pass/SkyboxRenderPass.hpp"

#include "render/CameraUniformBuffer.hpp"

#include "threading/ThreadSafeQueue.hpp"


#include <unordered_map>
#include <memory>
#include <queue>
#include <optional>

class Camera;
class ResourceManager;

struct RaycastResult;

class WorldRenderer
{
private:
	CameraUniformBuffer camera_uniform_buffer_;
	MeshRenderer mesh_renderer_;

	ChunkMeshRenderPass chunk_mesh_render_pass_;
	BlockHighlightRenderPass block_highlight_render_pass_;
	SkyboxRenderPass skybox_render_pass_;
	ChunkWireframeRenderPass chunk_wireframe_render_pass_;

	EventDispatcher<ChunkEvent> chunk_event_dispatcher_;

public:
	WorldRenderer();

	void Initialize();

	void SubscribeEvents();

	void Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue, const Camera& camera);

	void RenderWorld(const Camera& camera, float alpha, const ResourceManager& resource_manager);
};

#endif // WORLD_RENDERER_HPP

