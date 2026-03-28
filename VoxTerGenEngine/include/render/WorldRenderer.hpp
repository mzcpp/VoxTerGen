#ifndef WORLD_RENDERER_HPP
#define WORLD_RENDERER_HPP

#include "render/ChunkMeshRenderPass.hpp"
#include "world/World.hpp"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <unordered_map>
#include <memory>

class Chunk;
class ResourceManager;
struct ivec2_hash;

class WorldRenderer
{
private:
	ChunkMeshRenderPass chunk_mesh_render_pass_;

public:
	WorldRenderer();

	~WorldRenderer();

	void InitializeChunkRenderData(const World& world);

	void UploadChunkRenderData(const World& world);

	void RenderWorld(const World& world, const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager);

	void RenderChunks(
		const std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, ivec2_hash>& chunks, 
		const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager
	);
};

#endif

