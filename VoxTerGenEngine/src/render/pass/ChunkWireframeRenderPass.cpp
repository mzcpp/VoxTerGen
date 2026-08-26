#include "render/pass/ChunkWireframeRenderPass.hpp"

#include "core/ResourceManager.hpp"

#include "graphics/ShaderProgram.hpp"
#include "graphics/Camera.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/MeshBuilder.hpp"

#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

ChunkWireframeRenderPass::ChunkWireframeRenderPass(const MeshRenderer& mesh_renderer) : 
    mesh_renderer_(mesh_renderer), 
    render_wireframe_(true)
{
}

void ChunkWireframeRenderPass::PrepareChunkWireframeRenderData()
{
    chunk_wireframe_mesh_ = MeshBuilder::BuildChunkWireframeMesh();

	chunk_wireframe_mesh_render_data_.gpu_mesh_.InitializeBuffers();
	chunk_wireframe_mesh_render_data_.gpu_mesh_.UploadMeshData(chunk_wireframe_mesh_);
}

void ChunkWireframeRenderPass::RenderChunkWireframe(const Camera& camera, const ResourceManager& resource_manager)
{
    if (!render_wireframe_)
    {
        return;
    }

    const ShaderProgram* shader_program = resource_manager.GetShaderProgram("chunk_wireframe_shader");

	if (shader_program == nullptr)
    {
        return;
    }
	
	shader_program->Use();

	glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // find chunk_coords based on camera pos
    // calculate and set model matrix based on the chunk_coords
    // mesh_renderer_.RenderGpuMesh(chunk_wireframe_mesh_render_data_.gpu_mesh_);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glUseProgram(0);
}

