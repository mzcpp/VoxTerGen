#ifndef WORLD_RENDERER_HPP
#define WORLD_RENDERER_HPP

#include "render/ChunkMeshRenderPass.hpp"
#include "world/World.hpp"
#include "world/Chunk.hpp"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <unordered_map>
#include <memory>
#include <queue>

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

	void RenderWorld(std::queue<ChunkEvent>& chunk_event_queue, const World& world, const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager);
};

#endif

