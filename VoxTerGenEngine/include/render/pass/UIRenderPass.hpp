#ifndef UI_RENDER_PASS_HPP
#define UI_RENDER_PASS_HPP

#include "render/MeshRenderer.hpp"

class ResourceManager;

class UIRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;

public:
	UIRenderPass(const MeshRenderer& mesh_renderer);

	void PrepareCrosshairRenderData();

	void RenderCrosshair(const ResourceManager& resource_manager);
};

#endif // UI_RENDER_PASS_HPP

