#ifndef CHUNK_MESH_RENDER_PASS
#define CHUNK_MESH_RENDER_PASS

#include "core/ResourceManager.hpp"
#include "render/MeshRenderer.hpp"
#include "world/World.hpp"
#include "render/GpuMesh.hpp"
#include "utils/Hash.hpp"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <unordered_map>

struct ChunkRenderData
{
	GpuMesh gpu_mesh_;
	bool gpu_mesh_uploaded_ = false;
	glm::mat4 chunk_model_ = glm::mat4(1.0f);
};

class ChunkMeshRenderPass
{
private:
	MeshRenderer mesh_renderer_;
	std::unordered_map<glm::ivec2, ChunkRenderData, utils::ivec2_hash> chunk_render_data_;

public:
	void UpdateChunkRenderData(const World& world);

	void Render(const World& world, const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager);
	
	void UploadChunkRenderData(const World& world);

	void RenderChunks(const std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, utils::ivec2_hash>& chunks, const glm::mat4& view, 
		const glm::mat4& projection, const ResourceManager& resource_manager);

};

#endif

