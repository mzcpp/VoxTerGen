#include "world/ChunkManager.hpp"
#include "mesh/MeshBuilder.hpp"
#include "world/Chunk.hpp"
#include "core/Settings.hpp"
#include "utils/Logger.hpp"

std::size_t ivec2_hash::operator()(const glm::ivec2& vec) const noexcept
{
	const std::size_t h1 = std::hash<int>{}(vec.x);
	const std::size_t h2 = std::hash<int>{}(vec.y);
	return h1 ^ (h2 << 1);
}

ChunkManager::ChunkManager()
{
	InitChunks(constants::chunk::default_radius);
}

void ChunkManager::InitChunks(int chunk_radius)
{
	const int chunk_square_size = 2 * chunk_radius + 1;
	const glm::ivec3 start_coords = { 0 - chunk_radius, 0, 0 - chunk_radius };

	for (int z = 0; z < chunk_square_size; ++z)
	{
		for (int x = 0; x < chunk_square_size; ++x)
		{
			const glm::ivec2 world_coords = { start_coords.x + x, start_coords.z + z };
			std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(world_coords);
			InitChunkBlocks(*chunk);
			chunks_.emplace(world_coords, std::move(chunk));
		}
	}

	//chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 0 }).SetType(BlockType::Stone); // x
	//chunks_.begin()->second->BlockAt({ 0, 1, 1 }).SetType(BlockType::Stone); // x

	//chunks_.begin()->second->BlockAt({ 0, 0, 0} ).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 1} ).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 1, 0, 0} ).SetType(BlockType::Stone); // y
	//chunks_.begin()->second->BlockAt({ 1, 0, 1} ).SetType(BlockType::Stone); // y

	//chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 1, 0, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 0 }).SetType(BlockType::Stone); // z
	//chunks_.begin()->second->BlockAt({ 1, 1, 0 }).SetType(BlockType::Stone); // z

	// _____________
	// |X|X|A|X|X|A|
	// |X|X|A|X|X|A|
	// |A|A|X|X|A|A|
	// |A|A|X|X|A|A|
	// -------------
	//chunks_.begin()->second->BlockAt({ 0, 0, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 4 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 3, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 3, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 3, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 3, 4 }).SetType(BlockType::Stone);

	// _____________
	// |X|X|X|X|X|X|
	// |X|X|X|X|X|X|
	// |X|X|X|X|X|X|
	// |X|X|X|X|X|X|
	// -------------
	// for (int i = 0; i < 4; ++i)
	// {
	// 	for (int j = 0; j < 6; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, i, j }).SetType(BlockType::Stone);
	// 	}
	// }

	// _____________
	// |X|X|X|X|A|A|
	// |X|X|X|A|A|A|
	// |X|X|X|A|A|A|
	// |X|X|X|A|A|A|
	// -------------
	// for (int i = 0; i < 4; ++i)
	// {
	// 	for (int j = 0; j < 3; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, i, j }).SetType(BlockType::Stone);
	// 	}
	// }
	//chunks_.begin()->second->BlockAt({ 0, 3, 3 }).SetType(BlockType::Stone);
	
	// _____________
	// |X|X|X|X|X|X|
	// |X|X|X|C|C|C|
	// |X|X|X|C|C|C|
	// |X|X|X|C|C|C|
	// -------------
	// for (int i = 0; i < 4; ++i)
	// {
	// 	for (int j = 0; j < 3; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, i, j }).SetType(BlockType::Stone);
	// 	}
	// }
	//chunks_.begin()->second->BlockAt({ 0, 3, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 3, 4 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 3, 5 }).SetType(BlockType::Stone);
	// for (int i = 0; i < 3; ++i)
	// {
	// 	for (int j = 3; j < 5; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, i, j }).SetType(BlockType::Dirt);
	// 	}
	// }

	// _____________
	// |A|X|A|A|A|A|
	// |X|X|X|X|X|A|
	// |A|X|A|A|A|A|
	// |A|X|A|A|A|A|
	// -------------
	// for (int i = 0; i < 5; ++i)
	// {
	// 		chunks_.begin()->second->BlockAt({ 0, 2, i }).SetType(BlockType::Stone);
	// }
	// for (int j = 0; j < 4; ++j)
	// {
	// 	chunks_.begin()->second->BlockAt({ 0, j, 1 }).SetType(BlockType::Stone);
	// }
	
	// _____________
	// |X|X|X|A|A|A|
	// |X|A|A|A|A|A|
	// |X|A|A|A|A|A|
	// |X|X|X|X|X|X|
	// -------------
	// for (int i = 0; i < 6; ++i)
	// {
	// 		chunks_.begin()->second->BlockAt({ 0, 0, i }).SetType(BlockType::Stone);
	// }
	// for (int i = 0; i < 3; ++i)
	// {
	// 		chunks_.begin()->second->BlockAt({ 0, 3, i }).SetType(BlockType::Stone);
	// }
	// for (int i = 0; i < 4; ++i)
	// {
	// 		chunks_.begin()->second->BlockAt({ 0, i, 0 }).SetType(BlockType::Stone);
	// }

	// _____________
	// |X|X|X|X|X|X|
	// |X|A|A|A|A|X|
	// |X|A|A|A|A|X|
	// |X|X|X|X|X|X|
	// -------------
	// for (int i = 0; i < 6; ++i)
	// {
	// 		chunks_.begin()->second->BlockAt({ 0, 0, i }).SetType(BlockType::Stone);
	// 		chunks_.begin()->second->BlockAt({ 0, 3, i }).SetType(BlockType::Stone);
	// }
	// for (int i = 0; i < 3; ++i)
	// {
	// 		chunks_.begin()->second->BlockAt({ 0, i, 0 }).SetType(BlockType::Stone);
	// 		chunks_.begin()->second->BlockAt({ 0, i, 5 }).SetType(BlockType::Stone);
	// }

	// _____________
	// |A|A|A|A|A|A|
	// |A|X|A|X|A|X|
	// |A|A|A|A|A|A|
	// |X|A|X|A|X|A|
	// -------------
	//chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 4 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 5 }).SetType(BlockType::Stone);

	// _____________
	// |X|A|X|A|X|A|
	// |A|X|A|X|A|X|
	// |X|A|X|A|X|A|
	// |A|X|A|X|A|X|
	// -------------
	//chunks_.begin()->second->BlockAt({ 0, 0, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 5 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 4 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 5 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 3, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 3, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 3, 4 }).SetType(BlockType::Stone);

	// edges empty
	// _____________
	// |A|A|A|A|A|A|
	// |A|X|X|X|X|A|
	// |A|X|X|X|X|A|
	// |A|X|X|X|X|A|
	// -------------
	// for (int i = 1; i < 5; ++i)
	// {
	// 	for (int j = 0; j < 3; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, j, i }).SetType(BlockType::Stone);
	// 	}
	// }

	// L-shape
	// _____________
	// |A|A|A|A|A|A|
	// |X|X|A|A|A|A|
	// |X|X|X|X|A|A|
	// |A|A|X|X|X|A|
	// -------------
	//chunks_.begin()->second->BlockAt({ 0, 0, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 4 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 1 }).SetType(BlockType::Stone);

	// scattered blocks
	// _____________
	// |A|A|A|A|A|A|
	// |A|X|A|X|A|A|
	// |A|A|X|A|X|A|
	// |A|A|A|A|A|A|
	// -------------
	//chunks_.begin()->second->BlockAt({ 0, 1, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 4 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 3 }).SetType(BlockType::Stone);

	// edges filled
	// _____________
	// |X|X|X|X|X|X|
	// |X|A|A|A|A|X|
	// |X|A|X|X|A|X|
	// |X|X|X|X|X|X|
	// -------------
	// for (int i = 0; i < 6; ++i)
	// {
	// 		chunks_.begin()->second->BlockAt({ 0, 0, i }).SetType(BlockType::Stone);
	// 		chunks_.begin()->second->BlockAt({ 0, 3, i }).SetType(BlockType::Stone);
	// }
	// for (int i = 0; i < 3; ++i)
	// {
	// 		chunks_.begin()->second->BlockAt({ 0, i, 0 }).SetType(BlockType::Stone);
	// 		chunks_.begin()->second->BlockAt({ 0, i, 5 }).SetType(BlockType::Stone);
	// }
	//chunks_.begin()->second->BlockAt({ 0, 1, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 3 }).SetType(BlockType::Stone);

	// single row in middle
	// _____________
	// |A|A|A|A|A|A|
	// |A|A|A|A|A|A|
	// |X|X|X|X|A|A|
	// |A|A|A|A|A|A|
	// -------------
	//chunks_.begin()->second->BlockAt({ 0, 1, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 3 }).SetType(BlockType::Stone);

	// width 1 column
	// _____________
	// |A|A|A|A|A|A|
	// |X|A|A|A|A|A|
	// |X|A|A|A|A|A|
	// |X|A|A|A|A|A|
	// -------------
	//chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 0 }).SetType(BlockType::Stone);

	// _____________
	// |A|A|A|A|A|A|
	// |X|X|A|X|X|A|
	// |X|X|A|X|X|A|
	// |A|A|A|A|A|A|
	// -------------
	//chunks_.begin()->second->BlockAt({ 0, 1, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 4 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 4 }).SetType(BlockType::Stone);

	// _____________
	// |O|O|O|O|O|O|
	// |X|X|X|O|A|A|
	// |X|X|X|C|C|C|
	// |X|X|X|C|C|C|
	// -------------
	// for (int i = 0; i < 3; ++i)
	// {
	// 	for (int j = 0; j < 3; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, i, j }).SetType(BlockType::Stone);
	// 	}
	// }
	// for (int i = 0; i < 2; ++i)
	// {
	// 	for (int j = 0; j < 3; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, i, j }).SetType(BlockType::Dirt);
	// 	}
	// }
	// 	for (int j = 0; j < 6; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, 3, j }).SetType(BlockType::Sand);
	// 	}
	// 	chunks_.begin()->second->BlockAt({ 0, 2, 3 }).SetType(BlockType::Sand);

	// _____________
	// |O|O|O|O|O|O|
	// |X|A|C|C|C|C|
	// |X|X|B|C|C|C|
	// |X|X|X|C|C|C|
	// -------------
	// 	for (int j = 0; j < 6; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, 3, j }).SetType(BlockType::Sand);
	// 	}
	// for (int i = 0; i < 3; ++i)
	// {
	// 	for (int j = 0; j < 3; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, i, j }).SetType(BlockType::Snow);
	// 	}
	// }
	//chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Snow);
	//chunks_.begin()->second->BlockAt({ 0, 0, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 2 }).SetType(BlockType::Dirt);

	// _____________
	// |O|O|O|O|O|O|
	// |X|A|C|C|C|C|
	// |X|A|B|C|C|C|
	// |A|X|X|C|C|C|
	// -------------
	// 	for (int j = 0; j < 6; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, 3, j }).SetType(BlockType::Sand);
	// 	}
	// for (int i = 3; i < 6; ++i)
	// {
	// 	for (int j = 3; j < 6; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, i, j }).SetType(BlockType::Snow);
	// 	}
	// }
	//chunks_.begin()->second->BlockAt({ 0, 0, 1 }).SetType(BlockType::Snow);
	//chunks_.begin()->second->BlockAt({ 0, 0, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 2 }).SetType(BlockType::Dirt);

	// _____________
	// |O|O|O|O|O|O|
	// |O|O|X|O|O|A|
	// |A|X|X|A|A|A|
	// |X|X|X|X|X|A|
	// -------------
	// 	for (int j = 0; j < 6; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, 3, j }).SetType(BlockType::Sand);
	// 	}
	//chunks_.begin()->second->BlockAt({ 0, 2, 0 }).SetType(BlockType::Sand);
	//chunks_.begin()->second->BlockAt({ 0, 2, 1 }).SetType(BlockType::Sand);
	//chunks_.begin()->second->BlockAt({ 0, 2, 3 }).SetType(BlockType::Sand);
	//chunks_.begin()->second->BlockAt({ 0, 2, 4 }).SetType(BlockType::Sand);

	//chunks_.begin()->second->BlockAt({ 0, 0, 0 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 3 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 0, 4 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 1 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 1, 2 }).SetType(BlockType::Stone);
	//chunks_.begin()->second->BlockAt({ 0, 2, 2 }).SetType(BlockType::Stone);

	// _____________
	// |O|O|O|O|O|O|
	// |X|X|X|O|O|C|
	// |X|X|X|C|B|B|
	// |X|X|R|C|B|B|
	// -------------
	// 	for (int j = 0; j < 6; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, 3, j }).SetType(BlockType::Sand);
	// 	}
	//chunks_.begin()->second->BlockAt({ 0, 2, 3 }).SetType(BlockType::Sand);
	//chunks_.begin()->second->BlockAt({ 0, 2, 4 }).SetType(BlockType::Sand);
	// for (int i = 0; i < 3; ++i)
	// {
	// 	for (int j = 0; j < 3; ++j)
	// 	{
	// 		chunks_.begin()->second->BlockAt({ 0, i, j }).SetType(BlockType::Stone);
	// 	}
	// }
	//chunks_.begin()->second->BlockAt({ 0, 0, 2 }).SetType(BlockType::Dirt);

	//chunks_.begin()->second->BlockAt({ 0, 0, 3 }).SetType(BlockType::Snow);
	//chunks_.begin()->second->BlockAt({ 0, 1, 3 }).SetType(BlockType::Snow);
	//chunks_.begin()->second->BlockAt({ 0, 2, 5 }).SetType(BlockType::Snow);

	//chunks_.begin()->second->BlockAt({ 0, 0, 4 }).SetType(BlockType::Grass);
	//chunks_.begin()->second->BlockAt({ 0, 0, 5 }).SetType(BlockType::Grass);
	//chunks_.begin()->second->BlockAt({ 0, 1, 4 }).SetType(BlockType::Grass);
	//chunks_.begin()->second->BlockAt({ 0, 1, 5 }).SetType(BlockType::Grass);

	BuildAllChunkMeshes();
}

void ChunkManager::InitChunkBlocks(Chunk& chunk)
{
	for (int y = 0; y < constants::chunk::height; ++y)
	{
		for (int z = 0; z < constants::chunk::depth; ++z)
		{
			for (int x = 0; x < constants::chunk::width; ++x)
			{
				chunk.BlockAt({ x, y, z }).SetType(BlockType::Air);
			}
		}
	}
}

void ChunkManager::BuildAllChunkMeshes()
{
	for (auto& [world_coord, chunk] : chunks_)
	{
		std::unique_ptr<Mesh> chunk_mesh = std::make_unique<Mesh>();
		
		//*chunk_mesh = MeshBuilder::BuildMeshNaive(
		//	chunk->WorldCoords(),
		//	[this, &chunk](const glm::ivec3& block_coords)
		//	{
		//		return WorldBlockQuery(chunk->WorldCoords(), block_coords);
		//	}
		//);

		*chunk_mesh = MeshBuilder::BuildMeshGreedy(
			[this, &chunk](const glm::ivec3& block_coords)
			{
				return WorldBlockQuery(chunk->WorldCoords(), block_coords);
			}
		);

		chunk->SetMesh(std::move(chunk_mesh));
	}
}

const Chunk* ChunkManager::GetChunkAt(glm::ivec2 chunk_coord) const
{
	const auto it = chunks_.find(chunk_coord);

	if (it == chunks_.end())
	{
		return nullptr;
	}

	return it->second.get();
}

Block ChunkManager::WorldBlockQuery(glm::ivec2 current_chunk_coord, const glm::ivec3& block_coords) const
{
	if (block_coords.y < 0 || block_coords.y > constants::chunk::height - 1)
	{
		return Block();
	}

	const int x_chunk_offset = block_coords.x / constants::chunk::width;
	const int z_chunk_offset = block_coords.z / constants::chunk::depth;
	const Chunk* const target_chunk = GetChunkAt({ current_chunk_coord.x + x_chunk_offset, current_chunk_coord.y + z_chunk_offset });

	if (!target_chunk)
	{
		return Block();
	}
	
	int x_block_offset = block_coords.x % constants::chunk::width;
	int z_block_offset = block_coords.z % constants::chunk::depth;

	if (x_block_offset < 0)
	{
		x_block_offset += constants::chunk::width;
	}

	if (z_block_offset < 0)
	{
		z_block_offset += constants::chunk::depth;
	}
	
	return target_chunk->BlockAt({ x_block_offset, block_coords.y, z_block_offset });
}