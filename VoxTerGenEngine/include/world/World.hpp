#ifndef WORLD_HPP
#define WORLD_HPP

#include "world/Chunk.hpp"
#include "world/ChunkManager.hpp"

#include "threading/ThreadSafeQueue.hpp"

class ThreadPool;

class World
{
private:
	ChunkManager chunk_manager_;

public:
	World(Observer& observer, ThreadPool& thread_pool);

	void InitChunks(int chunk_radius);

	void Tick(ThreadSafeQueue<ChunkEvent>& chunk_event_queue);

	// Getters
	const ChunkManager& ChunkManagerRef() const { return chunk_manager_; }
	ChunkManager& ChunkManagerRef() { return chunk_manager_; }
};

#endif // WORLD_HPP
