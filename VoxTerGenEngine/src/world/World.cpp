#include "world/World.hpp"

World::World()
{

}

void World::Tick()
{
	chunk_manager_.Tick();
}
