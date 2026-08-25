#include "render/pass/ChunkTransparentRenderPass.hpp"

#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

#include "core/ResourceManager.hpp"

#include "mesh/Mesh.hpp"

#include "world/Chunk.hpp"

#include <glm/mat4x4.hpp>

#include <memory>
#include <ranges>
#include <unordered_map>

ChunkTransparentRenderPass::ChunkTransparentRenderPass(const MeshRenderer& mesh_renderer) : 
	mesh_renderer_(mesh_renderer)
{

}

void ChunkTransparentRenderPass::RenderTransparentChunkMeshes(const std::unordered_map<ChunkID, ChunkRenderData>& chunks_render_data, const ResourceManager& resource_manager)
{

}