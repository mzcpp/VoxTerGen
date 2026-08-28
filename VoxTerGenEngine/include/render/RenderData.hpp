#ifndef RENDER_DATA_HPP
#define RENDER_DATA_HPP

#include "physics/AABB.hpp"

#include "render/GpuMesh3D.hpp"

#include <glm/mat4x4.hpp>

struct Texture2DRenderData
{

};

struct ChunkMeshRenderData
{
    GpuMesh3D gpu_opaque_mesh_;
    GpuMesh3D gpu_transparent_mesh_;
    glm::mat4 model_matrix_ = glm::mat4(1.0f);
};

struct ChunkRenderData
{
	ChunkMeshRenderData mesh_render_data_;
	AABB aabb_;
    bool visible_ = false;
};

struct TransparentChunkData
{
	const ChunkRenderData* render_data_;
	double distance_squared_;
};

#endif // RENDER_DATA_HPP