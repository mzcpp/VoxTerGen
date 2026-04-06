#ifndef WORLD_HPP
#define WORLD_HPP

#include "world/Chunk.hpp"
#include "world/ChunkManager.hpp"

class World
{
private:
	ChunkManager chunk_manager_;

public:
	World();

	void InitChunks(int chunk_radius);

	void Tick(std::queue<ChunkEvent>& chunk_event_queue, const Camera& camera);

	// Getters
	const ChunkManager& ChunkManagerRef() const { return chunk_manager_; }
	ChunkManager& ChunkManagerRef() { return chunk_manager_; }
};

#endif // WORLD_HPP
