#include "render/BlockHighlightRenderPass.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/Direction.hpp"
#include "core/ResourceManager.hpp"

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
    constexpr float scale_factor = 1.001f;

    render_data_.model_matrix_ = glm::scale(render_data_.model_matrix_, glm::vec3(scale_factor));
    render_data_.model_matrix_ = glm::translate(render_data_.model_matrix_, glm::vec3(block_world_pos));
}

void BlockHighlightRenderPass::RenderBlockHighlight(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
    const ShaderProgram* shader_program = resource_manager.GetShaderProgram("block_highlight_shader");

    shader_program->Use();
	shader_program->Set<glm::mat4>("view", view);
	shader_program->Set<glm::mat4>("projection", projection);
    shader_program->Set<glm::mat4>("model", render_data_.model_matrix_);
    
    //mesh_renderer_.RenderGpuMesh(render_data_.gpu_mesh_);
}

