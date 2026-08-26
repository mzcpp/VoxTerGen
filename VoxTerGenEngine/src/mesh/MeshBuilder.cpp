#include "mesh/MeshBuilder.hpp"
#include "mesh/Vertex.hpp"
#include "mesh/Mesh.hpp"
#include "world/ChunkManager.hpp"
#include "world/Chunk.hpp"
#include "render/Material.hpp"
#include "utils/Logger.hpp"
#include "core/Direction.hpp"
#include "utils/Constants.hpp"
#include "utils/Timer.hpp"

#include <glm/vec3.hpp>

#include <array>
#include <cassert>

Mesh MeshBuilder::BuildUnitCubeMesh(BlockType block_type, glm::vec3 origin_offset)
{
	Mesh unit_cube_mesh;

	for (Direction dir : AllDirections())
    {
		CreateMeshIndices(unit_cube_mesh);
        CreateMeshVertices(block_type, dir, 1.0, origin_offset, unit_cube_mesh);
    }

	assert(unit_cube_mesh.Vertices().size() % 4 == 0);

	return unit_cube_mesh;
}

void MeshBuilder::CreateMeshIndices(Mesh& mesh)
{
	for (std::uint32_t i : { 0, 1, 2, 1, 3, 2 })
	{
		mesh.AddIndex(i + static_cast<std::uint32_t>(mesh.Vertices().size()));
	}
}

void MeshBuilder::CreateMeshVertices(BlockType type, Direction dir, float scale, glm::vec3 origin_offset, Mesh& mesh)
{
	for (int i = 0; i < 4; ++i)
	{
		Vertex vertex;

		switch (dir) 
		{
		case Direction::PosX:
			// 1, 0, 1
			// 1, 0, 0
			// 1, 1, 1
			// 1, 1, 0
			vertex.position_ = glm::vec3{ 1.0f, (i / 2) % 2 != 0, i % 2 == 0 };
			break;
		case Direction::NegX:
			// 0, 0, 0
			// 0, 0, 1
			// 0, 1, 0
			// 0, 1, 1
			vertex.position_ = glm::vec3{ 0.0f, (i / 2) % 2 != 0, i % 2 != 0 };
			break;
		case Direction::PosY:
			// 0, 1, 1
			// 1, 1, 1
			// 0, 1, 0
			// 1, 1, 0
			vertex.position_ = glm::vec3{ i % 2 != 0, 1.0f, (i / 2) % 2 == 0 };
			break;
		case Direction::NegY:
			// 1, 0, 1
			// 0, 0, 1
			// 1, 0, 0
			// 0, 0, 0
			vertex.position_ = glm::vec3{ i % 2 == 0, 0.0f, (i / 2) % 2 == 0 };
			break;
		case Direction::PosZ:
			// 0, 0, 1
			// 1, 0, 1
			// 0, 1, 1
			// 1, 1, 1
			vertex.position_ = glm::vec3{ i % 2 != 0, (i / 2) % 2 != 0, 1.0f };
			break;
		case Direction::NegZ:
			// 1, 0, 0
			// 0, 0, 0
			// 1, 1, 0
			// 0, 1, 0
			vertex.position_ = glm::vec3{ i % 2 == 0, (i / 2) % 2 != 0, 0.0f };
			break;
		default:
			assert(false);
			Logger::Log(LogLevel::ERROR, "CreateMeshVertices received an unknown type of Direction!: dir = {}", static_cast<std::uint8_t>(dir));
			return;
		}

		vertex.position_ *= scale;
		vertex.position_ += origin_offset;
		vertex.normal_ = static_cast<uint8_t>(dir);
		vertex.uv_ = glm::vec2{ i % 2 != 0, (i / 2) % 2 != 0 };
		vertex.material_ = GetQuadMaterial(type, dir);

		mesh.AddVertex(vertex);
	}
}

ChunkMesh MeshBuilder::BuildChunkMeshNaive(glm::ivec2 chunk_world_coords, const ChunkMeshDependencies& chunk_mesh_dependencies)
{
	ChunkMesh chunk_mesh;

	const Chunk* current_chunk = chunk_mesh_dependencies.GetChunk(glm::ivec2{ 0, 0 });
	const Chunk* west_chunk = chunk_mesh_dependencies.GetChunk(glm::ivec2{ -1, 0 });
	const Chunk* east_chunk = chunk_mesh_dependencies.GetChunk(glm::ivec2{ 1, 0 });
	const Chunk* north_chunk = chunk_mesh_dependencies.GetChunk(glm::ivec2{ 0, -1 });
	const Chunk* south_chunk = chunk_mesh_dependencies.GetChunk(glm::ivec2{ 0, 1 });

	for (int z = 0; z < constants::chunk::depth; ++z)
	{
		for (int y = 0; y < constants::chunk::height; ++y)
		{
			for (int x = 0; x < constants::chunk::width; ++x)
			{
				const glm::ivec3 block_coords = { x, y, z };
				const Block& current_block = current_chunk->BlockAt(block_coords);

				if (!current_block.IsSolid())
				{
					continue;
				}

				for (Direction dir : AllDirections())
				{
					glm::ivec3 neighbor_coords = NeighborCoords(block_coords, dir);
					const Chunk* neighbor_chunk = nullptr;

					if (neighbor_coords.x < 0)
					{
						neighbor_chunk = west_chunk;
						neighbor_coords += glm::ivec3{ constants::chunk::width, 0, 0 };
					}
					else if (neighbor_coords.x >= constants::chunk::width)
					{
						neighbor_chunk = east_chunk;
						neighbor_coords -= glm::ivec3{ constants::chunk::width, 0, 0 };
					}
					else if (neighbor_coords.y < 0 || neighbor_coords.y >= constants::chunk::height)
					{
						neighbor_chunk = nullptr;
					}
					else if (neighbor_coords.z < 0)
					{
						neighbor_chunk = north_chunk;
						neighbor_coords += glm::ivec3{ 0, 0, constants::chunk::depth };
					}
					else if (neighbor_coords.z >= constants::chunk::depth)
					{
						neighbor_chunk = south_chunk;
						neighbor_coords -= glm::ivec3{ 0, 0, constants::chunk::depth };
					}

					if (neighbor_chunk != nullptr && neighbor_chunk->BlockAt(neighbor_coords).IsSolid())
					{
						continue;
					}
					
					SaveQuadMesh(chunk_world_coords, current_block.Type(), block_coords, 1.0, dir, current_block.Type().IsTransparent() ? chunk_mesh.cpu_transparent_mesh_ : chunk_mesh.cpu_opaque_mesh_);
				}
			}
		}
	}

	return chunk_mesh;
}

ChunkMesh MeshBuilder::BuildChunkMeshGreedy(const ChunkMeshDependencies& chunk_mesh_dependencies, std::stop_token stop_token)
{
	ChunkMesh chunk_mesh;

	for (MajorAxis axis : AllAxes())
	{
		if (stop_token.stop_requested())
		{
			return ChunkMesh();
		}

		BuildAxisMesh(axis, chunk_mesh_dependencies, stop_token, chunk_mesh);
	}

	return chunk_mesh;
}

Mesh MeshBuilder::BuildChunkWireframeMesh()
{
	Mesh chunk_wireframe_mesh;

	constexpr int step = 2;

	static_assert(step > 0);
	static_assert(constants::chunk::width % step == 0);
	static_assert(constants::chunk::height % step == 0);
	static_assert(constants::chunk::depth % step == 0);

	for (int z = 0; z < constants::chunk::depth; z += step)
	{
		for (int y = 0; y < constants::chunk::height; y += step)
		{
			for (int x = 0; x < constants::chunk::width; x += step)
			{
				const BlockType type = BlockType::Air;
				const glm::ivec2 chunk_coords = { 0, 0 };
				const glm::ivec3 pos_x_block_coords = { constants::chunk::width - 2, y, z };
				const glm::ivec3 pos_z_block_coords = { x, y, constants::chunk::depth - 2 };

				const glm::ivec3 neg_x_block_coords = { -2, y, z };
				const glm::ivec3 neg_z_block_coords = { x, y, -2 };

				SaveQuadMesh(chunk_coords, type, pos_x_block_coords, static_cast<float>(step), Direction::PosX, chunk_wireframe_mesh);
				SaveQuadMesh(chunk_coords, type, pos_z_block_coords, static_cast<float>(step), Direction::PosZ, chunk_wireframe_mesh);

				SaveQuadMesh(chunk_coords, type, neg_x_block_coords, static_cast<float>(step), Direction::PosX, chunk_wireframe_mesh);
				SaveQuadMesh(chunk_coords, type, neg_z_block_coords, static_cast<float>(step), Direction::PosZ, chunk_wireframe_mesh);
			}
		}
	}

	return chunk_wireframe_mesh;
}

void MeshBuilder::SaveQuadMesh(glm::ivec2 chunk_world_coords, BlockType type, glm::ivec3 block_rel_coords, float scale, Direction dir, Mesh& mesh)
{
	const glm::vec3 block_abs_pos = {
		chunk_world_coords.x * constants::chunk::width + block_rel_coords.x,
		block_rel_coords.y,
		chunk_world_coords.y * constants::chunk::depth + block_rel_coords.z
	};

	CreateMeshIndices(mesh);
	CreateMeshVertices(type, dir, scale, block_abs_pos, mesh);
	
	assert(mesh.Vertices().size() % 4 == 0);
}

std::uint8_t MeshBuilder::GetQuadMaterial(BlockType block_type, Direction dir)
{
	switch (block_type)
	{
	case BlockType::Air:
		return static_cast<std::uint8_t>(Material::Air);

	case BlockType::Grass:
		if (dir == Direction::PosY)
		{
			return static_cast<std::uint8_t>(Material::GrassTop);
		}
		else if (dir == Direction::NegY)
		{
			return static_cast<std::uint8_t>(Material::Dirt);
		}
		else
		{
			return static_cast<std::uint8_t>(Material::GrassSide);
		}
	
	case BlockType::Dirt:
		return static_cast<std::uint8_t>(Material::Dirt);
	
	case BlockType::Stone:
		return static_cast<std::uint8_t>(Material::Stone);
	
	case BlockType::Water:
		return static_cast<std::uint8_t>(Material::Water);
	
	case BlockType::Sand:
		return static_cast<std::uint8_t>(Material::Sand);
	
	case BlockType::Snow:
		return static_cast<std::uint8_t>(Material::Snow);
	
	case BlockType::Bedrock:
		return static_cast<std::uint8_t>(Material::Bedrock);
	}

	assert(false);
	Logger::Log(LogLevel::ERROR, "GetQuadMaterial received an unknown type of Block!: block_type = {}", static_cast<std::uint8_t>(block_type));
	return static_cast<std::uint8_t>(Material::Air);
}

void MeshBuilder::BuildAxisMesh(MajorAxis major_axis, const ChunkMeshDependencies& chunk_mesh_dependencies, std::stop_token stop_token, ChunkMesh& chunk_mesh)
{
	int major_axis_size = 0;
	int cross_axis_1_size = 0;
	int cross_axis_2_size = 0;

	if (major_axis == MajorAxis::X)
	{
		major_axis_size = constants::chunk::width;
		cross_axis_1_size = constants::chunk::height;
		cross_axis_2_size = constants::chunk::depth;
	}
	else if (major_axis == MajorAxis::Y)
	{
		major_axis_size = constants::chunk::height;
		cross_axis_1_size = constants::chunk::depth;
		cross_axis_2_size = constants::chunk::width;
	}
	else
	{
		major_axis_size = constants::chunk::depth;
		cross_axis_1_size = constants::chunk::height;
		cross_axis_2_size = constants::chunk::width;
	}

	std::vector<MaskCell> slice_mask(cross_axis_1_size * cross_axis_2_size);

	for (int major_axis_index = -1; major_axis_index < major_axis_size; ++major_axis_index)
	{
		if (stop_token.stop_requested())
		{
			return;
		}

		BuildSliceMask(major_axis, major_axis_index, major_axis_size, cross_axis_1_size, cross_axis_2_size, chunk_mesh_dependencies, slice_mask);
		MergeFacesAndEmitData(major_axis, major_axis_index, cross_axis_2_size, cross_axis_1_size, slice_mask, chunk_mesh);
	}
}

bool MeshBuilder::MaskCellsMergable(const MaskCell& first, const MaskCell& second)
{
	if (first.block_type_ == BlockType::Air || second.block_type_ == BlockType::Air)
	{
		return false;
	}

	if (first.block_type_ != second.block_type_)
	{
		return false;
	}

	if (first.dir_ != second.dir_)
	{
		return false;
	}

	if (first.sun_light_ != second.sun_light_)
	{
		return false;
	}

	if (first.block_light_ != second.block_light_)
	{
		return false;
	}

	return true;
}

void MeshBuilder::BuildSliceMask(MajorAxis major_axis, int major_axis_index, int major_axis_size, int cross_axis_1_size, int cross_axis_2_size, const ChunkMeshDependencies& chunk_mesh_dependencies, std::vector<MaskCell>& slice_mask)
{
	const Chunk* current_chunk = chunk_mesh_dependencies.GetChunk(glm::ivec2{ 0, 0 });
	const Chunk* west_chunk = chunk_mesh_dependencies.GetChunk(glm::ivec2{ -1, 0 });
	const Chunk* east_chunk = chunk_mesh_dependencies.GetChunk(glm::ivec2{ 1, 0 });
	const Chunk* north_chunk = chunk_mesh_dependencies.GetChunk(glm::ivec2{ 0, -1 });
	const Chunk* south_chunk = chunk_mesh_dependencies.GetChunk(glm::ivec2{ 0, 1 });

	const Chunk* left_chunk = nullptr;
	const Chunk* right_chunk = nullptr;

	const bool left_block_inside = major_axis_index != -1;
	const bool right_block_inside = (major_axis_index + 1) != major_axis_size;
	
	for (int cross_axis_1_index = 0; cross_axis_1_index < cross_axis_1_size; ++cross_axis_1_index)
	{
		for (int cross_axis_2_index = 0; cross_axis_2_index < cross_axis_2_size; ++cross_axis_2_index)
		{
			glm::ivec3 left_query_coords(0);
			glm::ivec3 right_query_coords(0);
			
			switch (major_axis)
			{
			case MajorAxis::X:
			{
				left_chunk = left_block_inside ? current_chunk : west_chunk;
				right_chunk = right_block_inside ? current_chunk : east_chunk;
				
				left_query_coords = { major_axis_index, cross_axis_1_index, cross_axis_2_index };
				right_query_coords = { major_axis_index + 1, cross_axis_1_index, cross_axis_2_index };

				if (!left_block_inside)
				{
					left_query_coords += glm::ivec3{ constants::chunk::width, 0, 0 };
				}
				else if (!right_block_inside)
				{
					right_query_coords -= glm::ivec3{ constants::chunk::width, 0, 0 };
				}
				
				break;
			}
			case MajorAxis::Y:
			{
				left_chunk = left_block_inside ? current_chunk : nullptr;
				right_chunk = right_block_inside ? current_chunk : nullptr;

				left_query_coords = { cross_axis_2_index, major_axis_index, cross_axis_1_index };				
				right_query_coords = { cross_axis_2_index, major_axis_index + 1, cross_axis_1_index };
				
				break;
			}
			case MajorAxis::Z:
			{
				left_chunk = left_block_inside ? current_chunk : north_chunk;
				right_chunk = right_block_inside ? current_chunk : south_chunk;

				left_query_coords = { cross_axis_2_index, cross_axis_1_index, major_axis_index };
				right_query_coords = { cross_axis_2_index, cross_axis_1_index, major_axis_index + 1 };

				if (!left_block_inside)
				{
					left_query_coords += glm::ivec3{ 0, 0, constants::chunk::depth };
					
				}
				else if (!right_block_inside)
				{
					right_query_coords -= glm::ivec3{ 0, 0, constants::chunk::depth };
				}

				break;
			}
			default:
				assert(false && "Invalid major axis!");
				break;
			}

			const Block left_block = left_chunk != nullptr ? left_chunk->BlockAt(left_query_coords) : Block();
			const Block right_block = right_chunk != nullptr ? right_chunk->BlockAt(right_query_coords) : Block();
			
			const glm::ivec3 above_left_query_coords = left_query_coords + glm::ivec3{ 0, 1, 0 };
			const glm::ivec3 above_right_query_coords = right_query_coords + glm::ivec3{ 0, 1, 0 };

			const Block above_left_block = above_left_query_coords.y >= constants::chunk::height ? Block() : current_chunk->BlockAt(above_left_query_coords);
			const Block above_right_block = above_right_query_coords.y >= constants::chunk::height ? Block() : current_chunk->BlockAt(above_right_query_coords);

			const bool render_left = left_block_inside && left_block.ShouldRenderFace(right_block);
			const bool render_right = right_block_inside && right_block.ShouldRenderFace(left_block);

			MaskCell mask_cell = { BlockType::Air, Direction::PosX, 0, 0 };

			if (render_left)
			{
				mask_cell.block_type_ = (left_block.Type() == BlockType::Grass && above_left_block.IsSolid()) ? BlockType::Dirt : left_block.Type();
				mask_cell.dir_ = ToDirection(major_axis, true);
				mask_cell.sun_light_ = left_block.SunLight();
				mask_cell.block_light_ = left_block.BlockLight();
			}
			else if (render_right)
			{
				mask_cell.block_type_ = (right_block.Type() == BlockType::Grass && above_right_block.IsSolid()) ? BlockType::Dirt : right_block.Type();
				mask_cell.dir_ = ToDirection(major_axis, false);
				mask_cell.sun_light_ = right_block.SunLight();
				mask_cell.block_light_ = right_block.BlockLight();
			}

			slice_mask[cross_axis_1_index * cross_axis_2_size + cross_axis_2_index] = mask_cell;
		}
	}
}

void MeshBuilder::EmitVerticesAndIndices(MajorAxis major_axis, const MergedQuad& merged_quad, int major_axis_index, const MaskCell& first_merged_cell, ChunkMesh& chunk_mesh)
{
	const bool isMeshTransparent = first_merged_cell.block_type_.IsTransparent();
	const std::uint8_t normal = static_cast<std::uint8_t>(first_merged_cell.dir_);
	const std::uint8_t material = GetQuadMaterial(first_merged_cell.block_type_, first_merged_cell.dir_);

	const bool dir_is_even = static_cast<std::uint8_t>(first_merged_cell.dir_) % 2 == 0;
	const bool invert_j = ((major_axis == MajorAxis::X || major_axis == MajorAxis::Y) && dir_is_even) || (major_axis == MajorAxis::Z && !dir_is_even);

	for (std::uint32_t i : { 0, 1, 2, 1, 3, 2 })
	{
		if (isMeshTransparent)
		{
			chunk_mesh.cpu_transparent_mesh_.AddIndex(i + static_cast<std::uint32_t>(mesh.Vertices().size()));
		}
		else
		{
			chunk_mesh.cpu_opaque_mesh_.AddIndex(i + static_cast<std::uint32_t>(mesh.Vertices().size()));
		}
	}

	glm::vec3 vertex_position(0.0f);
	
	for (int i : { 0, 1 })
	{
		for (int j : { 0, 1 })
		{
			if (invert_j)
			{
				j = 1 - j;
			}

			const int x_pos = merged_quad.bottom_left_.x + (j * merged_quad.width_);
			const int y_pos = merged_quad.bottom_left_.y + (i * merged_quad.height_);

			if (major_axis == MajorAxis::X)
			{
				vertex_position = glm::vec3{ major_axis_index + 1, y_pos, x_pos };
			}
			else if (major_axis == MajorAxis::Y)
			{
				vertex_position = glm::vec3{ x_pos, major_axis_index + 1, y_pos };
			}
			else
			{
				vertex_position = glm::vec3{ x_pos, y_pos, major_axis_index + 1 };
			}

			const glm::vec2 uv = { j * merged_quad.width_, i * merged_quad.height_ };

			if (isMeshTransparent)
			{
				chunk_mesh.cpu_transparent_mesh_.AddVertex(vertex_position, normal, uv, material);
			}
			else
			{
				chunk_mesh.cpu_opaque_mesh_.AddVertex(vertex_position, normal, uv, material);
			}
		}
	}
}

void MeshBuilder::MergeFacesAndEmitData(MajorAxis major_axis, int major_axis_index, int mask_width, int mask_height, std::vector<MaskCell>& slice_mask, ChunkMesh& chunk_mesh)
{
	for (int y = 0; y < mask_height; ++y)
	{
		for (int x = 0; x < mask_width;)
		{
			const MaskCell& cell = slice_mask[y * mask_width + x];
			
			if (cell.block_type_ == BlockType::Air)
			{
				++x;
				continue;
			}

			int merged_quad_width = 1;

			while (x + merged_quad_width < mask_width && MaskCellsMergable(cell, slice_mask[y * mask_width + (x + merged_quad_width)]))
			{
				++merged_quad_width;
			}

			int merged_quad_height = 1;
			bool done = false;

			while (y + merged_quad_height < mask_height && !done)
            {
                for (int k = 0; k < merged_quad_width; ++k)
                {
                    if (!MaskCellsMergable(cell, slice_mask[(y + merged_quad_height) * mask_width + x + k]))
                    {
                        done = true;
                        break;
                    }
                }

                if (!done)
				{
                    ++merged_quad_height;
				}
            }

			MergedQuad merged_quad;
			merged_quad.bottom_left_.x = x;
			merged_quad.bottom_left_.y = y;
			merged_quad.width_ = merged_quad_width;
			merged_quad.height_ = merged_quad_height;
			
			EmitVerticesAndIndices(major_axis, merged_quad, major_axis_index, cell, chunk_mesh);

			for (int dy = 0; dy < merged_quad_height; ++dy)
            {
                for (int dx = 0; dx < merged_quad_width; ++dx)
                {
                    slice_mask[(y + dy) * mask_width + (x + dx)].block_type_ = BlockType::Air;
                }
            }

			x += merged_quad_width;
		}	
	}
}