#include "world/World.hpp"
#include "graphics/Camera.hpp"

World::World()
{

}

void World::Tick(const Camera& camera)
{
	chunk_manager_.Tick(camera);
}
