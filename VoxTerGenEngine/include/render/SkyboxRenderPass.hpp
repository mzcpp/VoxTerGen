#ifndef SKYBOX_RENDER_PASS_HPP
#define SKYBOX_RENDER_PASS_HPP

#include "render/MeshRenderer.hpp"
#include "render/MeshRenderData.hpp"

#include "world/Chunk.hpp"

#include <glm/mat4x4.hpp>

class Mesh;
class ResourceManager;

class SkyboxRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	Mesh skybox_mesh_;
	MeshRenderData render_data_;
	glm::mat4 view_;

public:
	SkyboxRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareSkyboxRenderData();

	void UpdateViewMatrix(const glm::mat4& camera_view);

	void RenderSkybox(const glm::mat4& projection, const ResourceManager& resource_manager);
};

#endif

