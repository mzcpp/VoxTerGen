#ifndef WORLD_RENDERER_HPP
#define WORLD_RENDERER_HPP

#include "render/MeshRenderer.hpp"
#include "world/World.hpp"
#include "render/GpuMesh.hpp"

#include <glm/glm.hpp>

#include <unordered_map>
#include <memory>

class Chunk;
class ResourceManager;
struct ivec2_hash;

struct ChunkRenderData
{
	GpuMesh gpu_mesh_;
	glm::mat4 chunk_model_ = glm::mat4(1.0f);
};

class WorldRenderer
{
private:
	MeshRenderer mesh_renderer_;
	std::unordered_map<glm::ivec2, ChunkRenderData, ivec2_hash> chunk_render_data_;

public:
	WorldRenderer();

	~WorldRenderer();

	void InitializeChunkRenderData();

	void UploadChunkRenderData(const World& world);

	void RenderWorld(const World& world, const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager);

	void RenderChunks(
		const std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, ivec2_hash>& chunks, 
		const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager
	);
};

#endif

