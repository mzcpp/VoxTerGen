#ifndef CHUNK_EVENTS_HPP
#define CHUNK_EVENTS_HPP

//#include "mesh/Mesh.hpp"

#include "render/MeshRenderData.hpp"

#include "world/Chunk.hpp"

#include <glm/vec2.hpp>

#include <memory>

class Mesh;

/**
 * @brief Helper struct to combine multiple lambdas for std::visit.
 * @note Taken from cppreference.com:
 *       https://en.cppreference.com/w/cpp/utility/variant/visit2.html
 */
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

namespace chunk_event
{
    struct ChunkMeshReady
    {
        ChunkID chunk_id_;
        glm::ivec2 world_coords_;
        std::unique_ptr<Mesh> cpu_mesh_;
    };

    struct ChunkDestroyed
    {
        ChunkID chunk_id_;
    };
}

using ChunkEvent = std::variant<chunk_event::ChunkMeshReady, chunk_event::ChunkDestroyed>;

#endif // CHUNK_EVENTS_HPP