#ifndef UI_RENDER_PASS_HPP
#define UI_RENDER_PASS_HPP

#include "mesh/Mesh2D.hpp"

#include "render/MeshRenderer.hpp"
#include "render/RenderData.hpp"

class ResourceManager;

struct ScreenDimensionsData;

class UIRenderPass
{
private:
	const MeshRenderer& mesh_renderer_;
	const ScreenDimensionsData& screen_dimensions_data_;

	Mesh2D crosshair_mesh_;
	Texture2DRenderData crosshair_render_data_;

public:
	UIRenderPass(const MeshRenderer& mesh_renderer, const ScreenDimensionsData& screen_dimensions_data);

	void PrepareCrosshairRenderData();

	void RenderCrosshair(const ResourceManager& resource_manager);
};

#endif // UI_RENDER_PASS_HPP

