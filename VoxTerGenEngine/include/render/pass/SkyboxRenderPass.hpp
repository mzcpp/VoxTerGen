#ifndef SKYBOX_RENDER_PASS_HPP
#define SKYBOX_RENDER_PASS_HPP

#include "mesh/Mesh.hpp"

#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

class ResourceManager;

class SkyboxRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	Mesh skybox_mesh_;
	MeshRenderData render_data_;

public:
	SkyboxRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareSkyboxRenderData();

	void RenderSkybox(const ResourceManager& resource_manager);
};

#endif

