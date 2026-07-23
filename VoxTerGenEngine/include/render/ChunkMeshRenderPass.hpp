#ifndef CHUNK_MESH_RENDER_PASS_HPP
#define CHUNK_MESH_RENDER_PASS_HPP

#include "core/ResourceManager.hpp"

#include "physics/AABB.hpp"

#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"

#include "world/Chunk.hpp"
#include "world/ChunkEvents.hpp"

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

struct ChunkData
{
	MeshRenderData mesh_render_data_;
	AABB aabb_;
};

class Camera;

class ChunkMeshRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	std::unordered_map<ChunkID, ChunkData> chunks_data_;

public:
	ChunkMeshRenderPass(const MeshRenderer& mesh_renderer);

	void ProcessChunkEvents(std::queue<ChunkEvent>& chunk_event_queue);

	void RenderChunks(const Camera& camera, float alpha, const ResourceManager& resource_manager);
};

#endif // CHUNK_MESH_RENDER_PASS_HPP

