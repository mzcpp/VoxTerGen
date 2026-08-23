#ifndef CHUNK_EVENTS_HPP
#define CHUNK_EVENTS_HPP

#include "world/Chunk.hpp"

#include <glm/vec2.hpp>

#include <memory>

class Mesh;

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

using ChunkEvent = std::variant<ChunkMeshReady, ChunkDestroyed>;

#endif // CHUNK_EVENTS_HPP