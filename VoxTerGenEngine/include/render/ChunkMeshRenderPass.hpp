#ifndef CHUNK_MESH_RENDER_PASS
#define CHUNK_MESH_RENDER_PASS

#include "core/ResourceManager.hpp"
#include "render/MeshRenderer.hpp"
#include "world/World.hpp"
#include "render/GpuMesh.hpp"
#include "utils/Hash.hpp"
#include "world/Chunk.hpp"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <unordered_map>
#include <queue>

struct ChunkRenderData
{
	GpuMesh gpu_mesh_;
	glm::mat4 chunk_model_ = glm::mat4(1.0f);
};

class ChunkMeshRenderPass
{
private:
	MeshRenderer mesh_renderer_;
	std::unordered_map<ChunkID, ChunkRenderData> chunks_render_data_;

public:
	void Render(std::queue<ChunkEvent>& chunk_event_queue, const World& world, const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager);
	
	void ProcessChunkEvents(std::queue<ChunkEvent>& chunk_event_queue);

	void RenderChunks(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager);

};

#endif

