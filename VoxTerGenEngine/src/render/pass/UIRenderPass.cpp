#include "render/pass/UIRenderPass.hpp"

#include "core/ResourceManager.hpp"

#include "graphics/ShaderProgram.hpp"

#include "mesh/MeshBuilder.hpp"

#include "render/MeshRenderer.hpp"

UIRenderPass::UIRenderPass(const MeshRenderer& mesh_renderer) : 
    mesh_renderer_(mesh_renderer)
{
}

void UIRenderPass::PrepareCrosshairRenderData()
{
    crosshair_mesh_ = MeshBuilder::BuildUnitMesh2D();
    crosshair_render_data_.gpu_mesh_.InitializeBuffers();
    crosshair_render_data_.gpu_mesh_.UploadMeshData(crosshair_mesh_);
}

void UIRenderPass::RenderCrosshair(const ResourceManager& resource_manager)
{
    const ShaderProgram* shader_program = resource_manager.GetShaderProgram("crosshair_shader");

    if (shader_program == nullptr)
    {
        return;
    }

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_program->Use();

    glActiveTexture(GL_TEXTURE0);
    resource_manager.GetTexture("crosshair")->Bind();

    shader_program->Set<int>("crosshair_texture", 0);

    mesh_renderer_.RenderGpuMesh<GpuMesh2D>(crosshair_render_data_.gpu_mesh_);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(0);
}

