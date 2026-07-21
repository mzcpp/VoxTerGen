#ifndef CHUNK_EVENTS_HPP
#define CHUNK_EVENTS_HPP

#include "world/Chunk.hpp"
#include "render/MeshRenderData.hpp"

#include <glm/vec2.hpp>

#include <memory>

class Mesh;

namespace chunk_event
{
    struct ChunkMeshReady
    {
        ChunkID chunk_id_;
        glm::ivec2 world_coords_;
        std::unique_ptr<Mesh> cpu_mesh_;
        MeshRenderData render_data_;
    };

    struct ChunkDestroyed
    {
        ChunkID chunk_id_;
    };
}

using ChunkEvent = std::variant<chunk_event::ChunkMeshReady, chunk_event::ChunkDestroyed>;

#endif