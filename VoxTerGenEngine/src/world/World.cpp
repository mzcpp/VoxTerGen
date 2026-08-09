#include "graphics/Camera.hpp"

#include "threading/ThreadPool.hpp"
#include "threading/ThreadSafeQueue.hpp"

#include "world/World.hpp"
#include "world/Chunk.hpp"

#include <queue>

World::World(ThreadPool& thread_pool) : 
	chunk_manager_(thread_pool)
{
}

void World::InitChunks(int chunk_radius)
{
	chunk_manager_.InitChunks(chunk_radius);
}

void World::Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue, const Camera& camera)
{
	chunk_manager_.Tick(chunk_event_queue, camera);
}
