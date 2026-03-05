#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "core/Settings.hpp"
#include "core/ResourceManager.hpp"
#include "world/World.hpp"
#include "input/InputManager.hpp"
#include "input/CameraController.hpp"
#include "graphics/Camera.hpp"
#include "render/WorldRenderer.hpp"

class Engine
{
private:
	InputManager input_manager_;
	Camera camera_;
	CameraController camera_controller_;
	ResourceManager resource_manager_;
	Settings settings_;
	World world_;
	WorldRenderer world_renderer_;

public:
	Engine();

	void HandleEvents(SDL_Event e);

	void Tick(float aspect_ratio);

	void Render(float alpha);

	const InputManager& InputManager() const { return input_manager_; }

	const Camera& Camera() const { return camera_; }

	const CameraController& CameraController() const { return camera_controller_; }
};

#endif

