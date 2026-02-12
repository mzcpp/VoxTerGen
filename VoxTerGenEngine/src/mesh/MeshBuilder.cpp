#include "mesh/MeshBuilder.hpp"
#include "mesh/Vertex.hpp"
#include "mesh/Mesh.hpp"
#include "world/ChunkManager.hpp"
#include "world/Chunk.hpp"
#include "render/Material.hpp"

#include <functional>
#include <array>
#include <cassert>

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

	const Block& block = chunk.BlockAt(x, y, z);
	std::array<Vertex, 4> quad_vertices;

	for (int i = 0; i < 4; ++i)
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		float normal_x = 0.0f;
		float normal_y = 0.0f;
		float normal_z = 0.0f;

		// 1, 0, 1
		// 1, 0, 0
		// 1, 1, 1
		// 1, 1, 0
		if (dir == Direction::PosX)
		{
			x = 1.0f;
			y = static_cast<float>((i / 2) % 2 != 0);
			z = static_cast<float>(i % 2 == 0);
			normal_x = 1.0f;
		}
		// 0, 0, 0
		// 0, 0, 1
		// 0, 1, 0
		// 0, 1, 1
		else if (dir == Direction::NegX)
		{
			x = static_cast<float>(i % 2 != 0);
			y = 1.0f;
			z = static_cast<float>((i / 2) % 2 == 0);
			normal_x = -1.0f;
		}
		// 0, 1, 1
		// 1, 1, 1
		// 0, 1, 0
		// 1, 1, 0
		else if (dir == Direction::PosY)
		{
			x = static_cast<float>(i % 2 != 0);
			y = 1.0f;
			z = static_cast<float>((i / 2) % 2 == 0);
			normal_y = 1.0f;
		}
		// 1, 0, 1
		// 0, 0, 1
		// 1, 0, 0
		// 0, 0, 0
		else if (dir == Direction::NegY)
		{
			x = static_cast<float>(i % 2 == 0);
			y = 0.0f;
			z = static_cast<float>((i / 2) % 2 == 0);
			normal_y = -1.0f;
		}
		// 0, 0, 1
		// 1, 0, 1
		// 0, 1, 1
		// 1, 1, 1
		else if (dir == Direction::PosZ)
		{
			x = static_cast<float>(i % 2 != 0);
			y = static_cast<float>((i / 2) % 2 != 0);
			z = 1.0f;
			normal_z = 1.0f;
		}
		// 1, 0, 0
		// 0, 0, 0
		// 1, 1, 0
		// 0, 1, 0
		else if (dir == Direction::NegZ)
		{
			x = static_cast<float>(i % 2 == 0);
			y = static_cast<float>((i / 2) % 2 != 0);
			z = 0.0f;
			normal_z = -1.0f;
		}

		quad_vertices[i].position_ = { x, y, z };
		quad_vertices[i].normal_ = { normal_x, normal_y, normal_z };
		quad_vertices[i].uv_ = { static_cast<float>(i % 2 != 0), static_cast<float>((i / 2) % 2 != 0) };
		quad_vertices[i].material_ = GetQuadMaterial(block.Type(), dir);
	}

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

uint8_t MeshBuilder::GetQuadMaterial(BlockType block_type, Direction dir)
{
	switch (block_type)
	{
		case BlockType::Air:
			return static_cast<uint8_t>(Material::Air);
		case BlockType::Grass:
			if (dir == Direction::PosY)
			{
				return static_cast<uint8_t>(Material::GrassTop);
			}
			else if (dir == Direction::NegY)
			{
				return static_cast<uint8_t>(Material::Dirt);
			}
			else
			{
				return static_cast<uint8_t>(Material::GrassSide);
			}
		case BlockType::Dirt:
			return static_cast<uint8_t>(Material::Dirt);
		case BlockType::Stone:
			return static_cast<uint8_t>(Material::Stone);
		case BlockType::Water:
			return static_cast<uint8_t>(Material::Water);
		case BlockType::Sand:
			return static_cast<uint8_t>(Material::Sand);
		case BlockType::Snow:
			return static_cast<uint8_t>(Material::Snow);
		default:
			assert(false);
	}
}