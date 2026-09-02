#ifndef SKYBOX_RENDER_PASS_HPP
#define SKYBOX_RENDER_PASS_HPP

#include "mesh/Mesh3D.hpp"

#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

class ResourceManager;

class SkyboxRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	Mesh3D skybox_mesh_;
	ChunkMeshRenderData render_data_;

public:
	SkyboxRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareSkyboxRenderData();

	void RenderSkybox(const ResourceManager& resource_manager);
};

#endif // SKYBOX_RENDER_PASS_HPP

