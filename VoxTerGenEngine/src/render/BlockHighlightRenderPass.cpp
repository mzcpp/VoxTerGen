#include "render/BlockHighlightRenderPass.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/Direction.hpp"
#include "core/ResourceManager.hpp"

#include "graphics/ShaderProgram.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/MeshBuilder.hpp"

#include "physics/DigitalDifferentialAnalyzer.hpp"

#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"

#include <optional>

BlockHighlightRenderPass::BlockHighlightRenderPass(const MeshRenderer& mesh_renderer) : 
    mesh_renderer_(mesh_renderer), 
    render_highlight_(false)
{
    block_highlight_mesh_ = MeshBuilder::BuildUnitCubeMesh(BlockType::Air, glm::vec3(-constants::geometry::block_center_offset));
}

void BlockHighlightRenderPass::PrepareBlockRenderData()
{
    render_data_.gpu_mesh_.InitializeBuffers();
    render_data_.gpu_mesh_.UploadMeshData(block_highlight_mesh_);
}

void BlockHighlightRenderPass::UpdateBlockHighlightModelMatrix(const std::optional<RaycastResult>& raycast_result)
{
    if (!raycast_result)
    {
        render_highlight_ = false;
        return;
    }

    constexpr float scale_factor = 1.001f;
    
    render_data_.model_matrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(raycast_result->block_coords_) + constants::geometry::block_center_offset);
    render_data_.model_matrix_ = glm::scale(render_data_.model_matrix_, glm::vec3(scale_factor));
    render_highlight_ = true;
}

void BlockHighlightRenderPass::RenderBlockHighlight(const glm::mat4& view, const glm::mat4& projection, const ResourceManager& resource_manager)
{
    if (!render_highlight_)
    {
        return;
    }

    const ShaderProgram* shader_program = resource_manager.GetShaderProgram("block_highlight_shader");

    if (shader_program == nullptr)
    {
        return;
    }

    shader_program->Use();
	shader_program->Set<glm::mat4>("view", view);
	shader_program->Set<glm::mat4>("projection", projection);
    shader_program->Set<glm::mat4>("model", render_data_.model_matrix_);

    constexpr float distance_threshold = 0.0025f;
    shader_program->Set<float>("distance_threshold", distance_threshold);
    
    mesh_renderer_.RenderGpuMesh(render_data_.gpu_mesh_);
}

