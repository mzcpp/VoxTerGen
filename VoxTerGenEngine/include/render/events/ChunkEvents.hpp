#ifndef CHUNK_EVENTS_HPP
#define CHUNK_EVENTS_HPP

#include "world/Chunk.hpp"

#include <glm/vec2.hpp>

#include <cstdint>
#include <memory>
#include <variant>

/**
 * @brief Helper struct to combine multiple lambdas for std::visit.
 * @note Taken from cppreference.com:
 *       https://en.cppreference.com/w/cpp/utility/variant/visit2.html
 */
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

struct ChunkMeshReady
{
    std::shared_ptr<Chunk> chunk_;
    std::uint64_t mesh_id_;
    std::unique_ptr<ChunkMesh> chunk_mesh_;
};

struct ChunkDestroyed
{
    ChunkID chunk_id_;
};

using ChunkEvent = std::variant<ChunkMeshReady, ChunkDestroyed>;

#endif // CHUNK_EVENTS_HPP