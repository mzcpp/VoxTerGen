#include "world/World.hpp"

World::World()
{

}

void World::Tick(const Camera& camera)
{
	chunk_manager_.Tick(camera);
}
