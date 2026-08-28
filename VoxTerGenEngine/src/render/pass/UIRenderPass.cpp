#include "render/pass/UIRenderPass.hpp"

#include "core/ResourceManager.hpp"

#include "graphics/ShaderProgram.hpp"

#include "render/MeshRenderer.hpp"

UIRenderPass::UIRenderPass(const MeshRenderer& mesh_renderer) : 
    mesh_renderer_(mesh_renderer)
{
}

void UIRenderPass::PrepareCrosshairRenderData()
{

}

void UIRenderPass::RenderCrosshair(const ResourceManager& resource_manager)
{

}

