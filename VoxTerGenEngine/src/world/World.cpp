#include "world/World.hpp"

#include "graphics/Camera.hpp"

#include "threading/ThreadPool.hpp"
#include "threading/ThreadSafeQueue.hpp"

#include "world/Chunk.hpp"

#include <queue>

World::World(Observer& observer, ThreadPool& thread_pool) :
	chunk_manager_(observer, thread_pool)
{
}

void World::InitChunks(int chunk_radius)
{
	chunk_manager_.InitChunks(chunk_radius);
}

void World::Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue)
{
	chunk_manager_.Tick(chunk_event_queue);
}
