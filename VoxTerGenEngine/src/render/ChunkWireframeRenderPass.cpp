#include "render/ChunkWireframeRenderPass.hpp"

#include "core/ResourceManager.hpp"

#include "graphics/ShaderProgram.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/MeshBuilder.hpp"

#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"

ChunkWireframeRenderPass::ChunkWireframeRenderPass(const MeshRenderer& mesh_renderer) : 
    mesh_renderer_(mesh_renderer), 
    render_wireframe_(false)
{
    
}

void ChunkWireframeRenderPass::PrepareChunkWireframeRenderData()
{

}

void ChunkWireframeRenderPass::RenderChunkWireframe(const ResourceManager& resource_manager)
{
    if (!render_wireframe_)
    {
        return;
    }
}

