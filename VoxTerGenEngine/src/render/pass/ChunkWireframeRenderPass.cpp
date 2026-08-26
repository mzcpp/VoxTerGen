#include "render/pass/ChunkWireframeRenderPass.hpp"

#include "core/ResourceManager.hpp"

#include "graphics/ShaderProgram.hpp"
#include "graphics/Camera.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/MeshBuilder.hpp"

#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

ChunkWireframeRenderPass::ChunkWireframeRenderPass(const MeshRenderer& mesh_renderer) : 
    mesh_renderer_(mesh_renderer), 
    render_wireframe_(false)
{
}

void ChunkWireframeRenderPass::PrepareChunkWireframeRenderData()
{
    chunk_wireframe_mesh_ = MeshBuilder::BuildChunkWireframeMesh();

	render_data_.gpu_transparent_mesh_.InitializeBuffers();
	render_data_.gpu_transparent_mesh_.UploadMeshData(chunk_wireframe_mesh_);
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

    const glm::ivec2 camera_chunk_pos = { std::floor(camera.Pos().x / constants::chunk::width), std::floor(camera.Pos().z / constants::chunk::depth) };
    const glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), { camera_chunk_pos.x * constants::chunk::width, 0, camera_chunk_pos.y * constants::chunk::depth });

    shader_program->Set<glm::mat4>("model", model_matrix);
    mesh_renderer_.RenderGpuMesh(render_data_.gpu_transparent_mesh_);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glUseProgram(0);
}

