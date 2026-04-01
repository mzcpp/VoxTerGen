#include "world/World.hpp"
#include "graphics/Camera.hpp"

World::World()
{

}

void World::Tick(std::queue<ChunkEvent>& chunk_event_queue, const Camera& camera)
{
	chunk_manager_.Tick(chunk_event_queue, camera);
}
