#include "world/World.hpp"
#include "world/Chunk.hpp"
#include "graphics/Camera.hpp"

#include <queue>

World::World()
{

}

void World::InitChunks(int chunk_radius)
{
	chunk_manager_.InitChunks(chunk_radius);
}


void World::Tick(std::queue<ChunkEvent>& chunk_event_queue, const Camera& camera)
{
	chunk_manager_.Tick(chunk_event_queue, camera);
}
