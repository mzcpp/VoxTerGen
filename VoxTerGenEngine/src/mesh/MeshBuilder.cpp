#include "mesh/MeshBuilder.hpp"
#include "mesh/Vertex.hpp"
#include "mesh/Mesh.hpp"
#include "world/ChunkManager.hpp"

#include <functional>
#include <array>

Mesh MeshBuilder::BuildMeshNaive(const Chunk& chunk, std::function<Block(glm::ivec2, int, int, int, Direction)> neighbor_query)
{
	Mesh chunk_mesh;
	const glm::ivec2& chunk_world_coords = chunk.WorldCoords();

	for (int z = 0; z < Constants::Chunk::depth; ++z)
	{
		for (int y = 0; y < Constants::Chunk::height; ++y)
		{
			for (int x = 0; x < Constants::Chunk::width; ++x)
			{
				if (!chunk.BlockAt(x, y, z).IsSolid())
				{
					continue;
				}

				for (Direction dir : AllDirections())
				{
					SaveQuadMesh(chunk, x, y, z, neighbor_query, dir, chunk_mesh);
				}
			}
		}	
	}

	return chunk_mesh;
}

Mesh MeshBuilder::BuildMeshGreedy(const Chunk& chunk, std::function<Block(glm::ivec2, int, int, int, Direction)> neighbor_query)
{
	Mesh result;
	return result;
}

void MeshBuilder::SaveQuadMesh(const Chunk& chunk, int x, int y, int z, std::function<Block(glm::ivec2, int, int, int, Direction)> neighbor_query, Direction dir, Mesh& chunk_mesh)
{
	const glm::ivec2& chunk_world_coords = chunk.WorldCoords();
	const Block neighbor = neighbor_query(chunk_world_coords, x, y, z, dir);
				
	if (neighbor.IsSolid())
	{
		return;
	}

	std::array<Vertex, 4> quad_vertices;

	switch (dir)
	{
		case Direction::PosX:
			// 1, 0, 1
			// 1, 0, 0
			// 1, 1, 1
			// 1, 1, 0
			
			for (int i = 0; i < 4; ++i)
			{
				quad_vertices[i].position_ = { 1.0f, static_cast<float>((i / 2) % 2 != 0), static_cast<float>(i % 2 == 0) };
				quad_vertices[i].normal_ = { 1.0f, 0.0f, 0.0f };
				quad_vertices[i].uv_ = { static_cast<float>(i % 2 != 0), static_cast<float>((i / 2) % 2 != 0) };
			}

			break;

		case Direction::NegX:
			// 0, 0, 0
			// 0, 0, 1
			// 0, 1, 0
			// 0, 1, 1

			for (int i = 0; i < 4; ++i)
			{
				quad_vertices[i].position_ = { 0.0f, static_cast<float>((i / 2) % 2 != 0), static_cast<float>(i % 2 != 0) };
				quad_vertices[i].normal_ = { -1.0f, 0.0f, 0.0f };
				quad_vertices[i].uv_ = { static_cast<float>(i % 2 != 0), static_cast<float>((i / 2) % 2 != 0) };
			}

			break;

		case Direction::PosY:
			// 0, 1, 1
			// 1, 1, 1
			// 0, 1, 0
			// 1, 1, 0

			for (int i = 0; i < 4; ++i)
			{
				quad_vertices[i].position_ = { static_cast<float>(i % 2 != 0), 1.0f, static_cast<float>((i / 2) % 2 == 0) };
				quad_vertices[i].normal_ = { 0.0f, 1.0f, 0.0f };
				quad_vertices[i].uv_ = { static_cast<float>(i % 2 != 0), static_cast<float>((i / 2) % 2 != 0) };
			}

			break;

		case Direction::NegY:
			// 1, 0, 1
			// 0, 0, 1
			// 1, 0, 0
			// 0, 0, 0

			for (int i = 0; i < 4; ++i)
			{
				quad_vertices[i].position_ = { static_cast<float>(i % 2 == 0), 0.0f, static_cast<float>((i / 2) % 2 == 0) };
				quad_vertices[i].normal_ = { 0.0f, -1.0f, 0.0f };
				quad_vertices[i].uv_ = { static_cast<float>(i % 2 != 0), static_cast<float>((i / 2) % 2 != 0) };
			}

			break;

		case Direction::PosZ:
			// 0, 0, 1
			// 1, 0, 1
			// 0, 1, 1
			// 1, 1, 1

			for (int i = 0; i < 4; ++i)
			{
				quad_vertices[i].position_ = { static_cast<float>(i % 2 != 0), static_cast<float>((i / 2) % 2 != 0), 1.0f };	
				quad_vertices[i].normal_ = { 0.0f, 0.0f, 1.0f };
				quad_vertices[i].uv_ = { static_cast<float>(i % 2 != 0), static_cast<float>((i / 2) % 2 != 0) };
			}

			break;

		case Direction::NegZ:
			// 1, 0, 0
			// 0, 0, 0
			// 1, 1, 0
			// 0, 1, 0

			for (int i = 0; i < 4; ++i)
			{
				quad_vertices[i].position_ = { static_cast<float>(i % 2 == 0), static_cast<float>((i / 2) % 2 != 0), 0.0f };
				quad_vertices[i].normal_ = { 0.0f, 0.0f, -1.0f };
				quad_vertices[i].uv_ = { static_cast<float>(i % 2 != 0), static_cast<float>((i / 2) % 2 != 0) };
			}

			break;
	}

	// MATERIAL!!!



	// Block at (x=3, y=5, z=2) in chunk (chunk_x=10, chunk_z=15)
	// Block size = 1
	// Chunk size = 16x16x128

	// world_x = chunk_x * 16 + x = 10*16 + 3 = 163
	// world_y = y = 5
	// world_z = chunk_z * 16 + z = 15*16 + 2 = 242


	const float world_x = chunk_world_coords.x * Constants::Chunk::width + x;
	const float world_y = y;
	const float world_z = chunk_world_coords.y * Constants::Chunk::depth + z;
	// OFFSET THE VERTICES POSITIONS

	const std::array<int, 6> quad_indices = {{ 0, 1, 2, 1, 3, 2 }};


	// save the vertices in world_coords
	// ++quads_saved;
	// save the indices
	// push_back(quad_indices[i] + (4 * quads_saved))
}