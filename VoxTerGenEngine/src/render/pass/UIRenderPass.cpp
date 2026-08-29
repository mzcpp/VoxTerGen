#include "render/pass/UIRenderPass.hpp"

#include "core/Application.hpp"
#include "core/ResourceManager.hpp"

#include "graphics/ShaderProgram.hpp"

#include "mesh/MeshBuilder.hpp"

#include "render/MeshRenderer.hpp"

UIRenderPass::UIRenderPass(const MeshRenderer& mesh_renderer, const ScreenDimensionsData& screen_dimensions_data) :
    mesh_renderer_(mesh_renderer),
    screen_dimensions_data_(screen_dimensions_data), 
    projection_(0.0f)
{
}

void UIRenderPass::PrepareCrosshairRenderData(const ResourceManager& resource_manager)
{
    const texture_utils::Texture2D* texture = resource_manager.GetTexture("crosshair");

    if (texture == nullptr)
    {
        return;
    }

    crosshair_mesh_ = MeshBuilder::BuildUnitMesh2D(glm::vec2{ texture->Width(), texture->Height() }, glm::vec2(0.0));
    crosshair_render_data_.gpu_mesh_.InitializeBuffers();
    crosshair_render_data_.gpu_mesh_.UploadMeshData(crosshair_mesh_);
}

void UIRenderPass::UpdateProjection()
{
    projection_ = glm::ortho(
        0.0f, 
        static_cast<float>(screen_dimensions_data_.screen_width_), 
        static_cast<float>(screen_dimensions_data_.screen_height_), 
        0.0f
    );
}

void UIRenderPass::RenderCrosshair(const ResourceManager& resource_manager)
{
    const ShaderProgram* shader_program = resource_manager.GetShaderProgram("crosshair_shader");
    const texture_utils::Texture2D* texture = resource_manager.GetTexture("crosshair");

    if (shader_program == nullptr || texture == nullptr)
    {
        return;
    }

    const float half_width = static_cast<float>(texture->Width()) * 0.5f;
    const float half_height = static_cast<float>(texture->Height()) * 0.5f;

    const glm::mat4 model = glm::translate(
        glm::mat4(1.0f),
        glm::vec3{
            static_cast<float>(screen_dimensions_data_.screen_width_) * 0.5f - half_width,
            static_cast<float>(screen_dimensions_data_.screen_height_) * 0.5f - half_height,
            0.0f
        }
    );

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_program->Use();

    glActiveTexture(GL_TEXTURE0);
    texture->Bind();

    shader_program->Set<int>("crosshair_texture", 0);
    shader_program->Set<glm::mat4>("projection", projection_);
    shader_program->Set<glm::mat4>("model", model);

    mesh_renderer_.RenderGpuMesh<GpuMesh2D>(crosshair_render_data_.gpu_mesh_);

    glDisable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(0);
}

