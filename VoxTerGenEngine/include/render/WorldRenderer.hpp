#ifndef WORLD_RENDERER_HPP
#define WORLD_RENDERER_HPP

#include "render/events/ChunkEvents.hpp"

#include "render/pass/BlockHighlightRenderPass.hpp"
#include "render/pass/ChunkOpaqueRenderPass.hpp"
#include "render/pass/ChunkTransparentRenderPass.hpp"
#include "render/pass/ChunkWireframeRenderPass.hpp"
#include "render/pass/SkyboxRenderPass.hpp"

#include "render/CameraUniformBuffer.hpp"

#include "threading/ThreadSafeQueue.hpp"

#include "world/Chunk.hpp"

#include <unordered_map>
#include <vector>

class Camera;
class ResourceManager;

struct ChunkRenderData;

class WorldRenderer
{
private:
	const Camera& camera_;
	
	CameraUniformBuffer camera_uniform_buffer_;
	MeshRenderer mesh_renderer_;

	ChunkOpaqueRenderPass chunk_opaque_render_pass_;
	BlockHighlightRenderPass block_highlight_render_pass_;
	SkyboxRenderPass skybox_render_pass_;
	ChunkWireframeRenderPass chunk_wireframe_render_pass_;
	ChunkTransparentRenderPass chunk_transparent_render_pass_;

	std::unordered_map<ChunkID, ChunkRenderData> chunks_render_data_;
	std::vector<TransparentChunkData> transparent_chunks_data_;

public:
	WorldRenderer(const Camera& camera);

	void Initialize();

	void ProcessChunkEvents(ThreadSafeQueue<ChunkEvent>& chunk_event_queue);

	void ProcessChunkMeshReady(const ChunkMeshReady& event);

	void ProcessChunkDestroyed(const ChunkDestroyed& event);

	void Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue);

	void RenderWorld(float alpha, const ResourceManager& resource_manager);

	void UpdateChunksVisibility();

	void UpdateTransparentChunks();
};

#endif // WORLD_RENDERER_HPP

