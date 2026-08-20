#include "render/pass/ChunkTransparentRenderPass.hpp"

#include "core/ResourceManager.hpp"

#include "graphics/ShaderProgram.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/MeshBuilder.hpp"

#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"

ChunkTransparentRenderPass::ChunkTransparentRenderPass(const MeshRenderer& mesh_renderer) :
    mesh_renderer_(mesh_renderer)
{
    
}

void ChunkTransparentRenderPass::RenderTransparentChunks(const ResourceManager& resource_manager)
{
}

