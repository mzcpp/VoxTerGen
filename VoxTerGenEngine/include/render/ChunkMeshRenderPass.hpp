#ifndef CHUNK_MESH_RENDER_PASS
#define CHUNK_MESH_RENDER_PASS

#include "core/ResourceManager.hpp"
#include "render/MeshRenderer.hpp"
#include "world/Chunk.hpp"

#include <glm/mat4x4.hpp>

#include <unordered_map>
#include <queue>

/**
 * @brief Helper struct to combine multiple lambdas for std::visit.
 * @note Taken from cppreference.com:
 *       https://en.cppreference.com/w/cpp/utility/variant/visit2.html
 */
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

class ChunkMeshRenderPass
{
private:
	MeshRenderer mesh_renderer_;
	std::unordered_map<ChunkID, ChunkRenderData> chunks_render_data_;

public:
	void ProcessChunkEvents(std::queue<ChunkEvent>& chunk_event_queue);

	void RenderChunks(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager);

};

#endif

