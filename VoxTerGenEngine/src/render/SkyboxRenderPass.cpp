#include "render/SkyboxRenderPass.h"

#include "core/ResourceManager.hpp"

#include "graphics/ShaderProgram.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/MeshBuilder.hpp"

#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

SkyboxRenderPass::SkyboxRenderPass(const MeshRenderer& mesh_renderer) : 
    mesh_renderer_(mesh_renderer), 
    view_(0.0f)
{
    skybox_mesh_ = MeshBuilder::BuildUnitCubeMesh(BlockType::Air);
}

void SkyboxRenderPass::PrepareSkyboxRenderData()
{
    render_data_.gpu_mesh_.InitializeBuffers();
    render_data_.gpu_mesh_.UploadMeshData(skybox_mesh_);
}

void SkyboxRenderPass::UpdateViewMatrix(const glm::mat4& camera_view)
{
    view_ = glm::mat4(glm::mat3(camera_view));
}

void SkyboxRenderPass::RenderSkybox(const glm::mat4& projection, const ResourceManager& resource_manager)
{
    const ShaderProgram* shader_program = resource_manager.GetShaderProgram("skybox_shader");

    if (shader_program == nullptr)
    {
        return;
    }

    shader_program->Use();
	shader_program->Set<glm::mat4>("view", view_);
	shader_program->Set<glm::mat4>("projection", projection);

    glActiveTexture(GL_TEXTURE0);
	resource_manager.GetTexture("sky_cubemap")->Bind();
	shader_program->Set<int>("skybox", 0);

    mesh_renderer_.RenderGpuMesh(render_data_.gpu_mesh_);
}
