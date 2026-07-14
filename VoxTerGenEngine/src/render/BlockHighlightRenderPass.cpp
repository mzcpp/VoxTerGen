#include "render/BlockHighlightRenderPass.hpp"

#include <glm/vec3.hpp>

#include "core/Direction.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/MeshBuilder.hpp"

#include "render/MeshRenderer.hpp"

BlockHighlightRenderPass::BlockHighlightRenderPass(const MeshRenderer& mesh_renderer) : 
    mesh_renderer_(mesh_renderer)
{
    for (Direction dir : AllDirections())
    {
        MeshBuilder::SaveQuadMesh({ 0, 0 }, BlockType::Air, { 0.0, 0.0, 0.0 }, dir, block_highlight_mesh_);
    }
}

void BlockHighlightRenderPass::PrepareBlockRenderData()
{
    render_data_.gpu_mesh_.InitializeBuffers();
    render_data_.gpu_mesh_.UploadMeshData(block_highlight_mesh_);
}

void BlockHighlightRenderPass::UpdateBlockHighlightModelMatrix(glm::ivec3 block_world_pos)
{
    render_data_.model_matrix_ = glm::scale(render_data_.model_matrix_, glm::vec3(1.001));
    render_data_.model_matrix_ = glm::translate(render_data_.model_matrix_, block_world_pos);
}

void BlockHighlightRenderPass::RenderBlockHighlight(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
    const ShaderProgram* shader_program = resource_manager.GetShaderProgram("block_highlight_shader");

    shader_program->Use();
	shader_program->Set<glm::mat4>("view", view);
	shader_program->Set<glm::mat4>("projection", projection);
    shader_program->Set<glm::mat4>("model", chunk_render_data.model_matrix_);
    
    mesh_renderer_.RenderGpuMesh(chunk_render_data.gpu_mesh_);
}

