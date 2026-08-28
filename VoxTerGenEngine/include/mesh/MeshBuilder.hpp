#ifndef MESH_BUILDER_HPP
#define MESH_BUILDER_HPP

#include "core/Direction.hpp"

#include "mesh/Mesh3D.hpp"

#include "render/Material.hpp"
#include "render/events/ChunkEvents.hpp"

#include "utils/constants.hpp"

#include "world/Chunk.hpp"
#include "world/Block.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <concepts>
#include <cstdint>
#include <stop_token>
#include <vector>

struct MaskCell
{
	BlockType block_type_;
	Direction dir_;
	std::uint8_t sun_light_;
	std::uint8_t block_light_;
};

struct MergedQuad
{
	glm::ivec2 bottom_left_;
	int width_;
	int height_;
};

class MeshBuilder final
{
public:
	MeshBuilder() = delete;

	MeshBuilder(const MeshBuilder& other) = delete;
	MeshBuilder& operator=(const MeshBuilder& other) = delete;

	static Mesh3D BuildUnitCubeMesh(BlockType block_type, glm::vec3 origin_offset = { 0.0f, 0.0f, 0.0f });

	static void CreateMeshIndices(Mesh3D& mesh);

	static void CreateMeshVertices(BlockType type, Direction dir, float scale, glm::vec3 origin_offset, Mesh3D& mesh);

	static ChunkMesh BuildChunkMeshNaive(glm::ivec2 chunk_world_coords, const ChunkMeshDependencies& chunk_mesh_dependencies);
	
	static ChunkMesh BuildChunkMeshGreedy(const ChunkMeshDependencies& chunk_mesh_dependencies, std::stop_token stop_token);

	static Mesh3D BuildChunkWireframeMesh();

	static void SaveQuadMesh(glm::ivec2 chunk_world_coords, BlockType type, glm::ivec3 block_rel_coords, float scale, Direction dir, Mesh3D& mesh);

	static std::uint8_t GetQuadMaterial(BlockType block_type, Direction dir);

	static void BuildAxisMesh(MajorAxis major_axis, const ChunkMeshDependencies& chunk_mesh_dependencies, std::stop_token stop_token, ChunkMesh& chunk_mesh);

	static bool MaskCellsMergable(const MaskCell& first, const MaskCell& second);

	static void BuildSliceMask(MajorAxis major_axis, int major_axis_index, int major_axis_size, int cross_axis_1_size, int cross_axis_2_size, const ChunkMeshDependencies& chunk_mesh_dependencies, std::vector<MaskCell>& slice_mask);
	
	static void EmitVerticesAndIndices(MajorAxis major_axis, const MergedQuad& merged_quad, int major_axis_index, const MaskCell& first_merged_cell, ChunkMesh& chunk_mesh);

	static void MergeFacesAndEmitData(MajorAxis major_axis, int major_axis_index, int mask_width, int mask_height, std::vector<MaskCell>& slice_mask, ChunkMesh& chunk_mesh);
};

#endif // MESH_BUILDER_HPP