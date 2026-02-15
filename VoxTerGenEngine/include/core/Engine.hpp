#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "core/Settings.hpp"
#include "world/World.hpp"
#include "input/InputManager.hpp"
#include "input/CameraController.hpp"
#include "graphics/Camera.hpp"

class Engine
{
private:
	InputManager input_manager_;
	Camera camera_;
	CameraController camera_controller_;
	Settings settings_;
	World world_;

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

