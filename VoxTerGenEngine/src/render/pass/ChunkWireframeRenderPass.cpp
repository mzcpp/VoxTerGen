#include "render/pass/ChunkWireframeRenderPass.hpp"

#include "core/ResourceManager.hpp"

#include "graphics/ShaderProgram.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/MeshBuilder.hpp"

#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

ChunkWireframeRenderPass::ChunkWireframeRenderPass(const MeshRenderer& mesh_renderer) : 
    mesh_renderer_(mesh_renderer), 
    render_wireframe_(false)
{
}

void ChunkWireframeRenderPass::PrepareChunkWireframeRenderData()
{
    chunk_wireframe_mesh_ = MeshBuilder::BuildChunkWireframeMesh();
}

void ChunkWireframeRenderPass::RenderChunkWireframe(const std::unordered_map<ChunkID, ChunkRenderData>& chunks_render_data, const std::array<Plane, 6>& frustum_planes, const ResourceManager& resource_manager)
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

	const auto inside_frustum = [&frustum_planes](const ChunkRenderData& chunk_data)
	{
		return geometry::Intersects(frustum_planes, chunk_data.aabb_);
	};

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // VERIFY THE MESH OF WIREFRAME IS CORRECT!
    
	// for (const ChunkRenderData& chunk_data : chunks_render_data | std::views::values | std::views::filter(inside_frustum))
	// {
	// 	shader_program->Set<glm::mat4>("model", chunk_data.mesh_render_data_.model_matrix_);
	// 	mesh_renderer_.RenderGpuMesh(chunk_data.mesh_render_data_.gpu_mesh_);
	// }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(0);
}

