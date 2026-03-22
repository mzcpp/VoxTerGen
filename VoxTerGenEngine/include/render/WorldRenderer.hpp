#ifndef WORLD_RENDERER_HPP
#define WORLD_RENDERER_HPP

#include "render/MeshRenderer.hpp"

#include <glm/glm.hpp>

#include <unordered_map>
#include <memory>

class Chunk;
class ResourceManager;
struct ivec2_hash;

class WorldRenderer
{
private:
	MeshRenderer mesh_renderer_;

public:
	WorldRenderer();

	~WorldRenderer();

	void Initialize();

	void RenderChunks(
		const std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, ivec2_hash>& chunks, 
		const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager
	);
};

#endif

