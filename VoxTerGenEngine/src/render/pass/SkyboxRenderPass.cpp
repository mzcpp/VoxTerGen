#include "render/pass/SkyboxRenderPass.hpp"

#include "core/ResourceManager.hpp"

#include "graphics/ShaderProgram.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/MeshBuilder.hpp"

#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

SkyboxRenderPass::SkyboxRenderPass(const MeshRenderer& mesh_renderer) : 
    mesh_renderer_(mesh_renderer)
{
}

void SkyboxRenderPass::PrepareSkyboxRenderData()
{
    skybox_mesh_ = MeshBuilder::BuildUnitCubeMesh(BlockType::Air, glm::vec3(-constants::geometry::block_center_offset));
    
    render_data_.gpu_mesh_.InitializeBuffers();
    render_data_.gpu_mesh_.UploadMeshData(skybox_mesh_);
}

void SkyboxRenderPass::RenderSkybox(const ResourceManager& resource_manager)
{
    const ShaderProgram* shader_program = resource_manager.GetShaderProgram("skybox_shader");

    if (shader_program == nullptr)
    {
        return;
    }

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    shader_program->Use();
    glActiveTexture(GL_TEXTURE0);
	resource_manager.GetTexture("sky_cubemap")->Bind();
	shader_program->Set<int>("skybox", 0);

    mesh_renderer_.RenderGpuMesh(render_data_.gpu_mesh_);

    glDepthFunc(GL_LESS);
    glCullFace(GL_BACK);
    glUseProgram(0);
}

