#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "core/Settings.hpp"
#include "core/ResourceManager.hpp"
#include "world/World.hpp"
#include "input/InputManager.hpp"
#include "input/CameraController.hpp"
#include "graphics/Camera.hpp"
#include "render/WorldRenderer.hpp"

#include <queue>

union SDL_Event;

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

	std::queue<ChunkEvent> chunk_event_queue_;

public:
	Engine();

	void Initialize();

	void HandleEvents(SDL_Event e);

	void Tick(float aspect_ratio);

	void Render(float alpha);

	// Getters
	const InputManager& InputManager() const noexcept { return input_manager_; }
	const Camera& Camera() const noexcept { return camera_; }
	const CameraController& CameraController() const noexcept { return camera_controller_; }
	const ResourceManager& ResourceManager() const noexcept { return resource_manager_; }
	const Settings& Settings() const noexcept { return settings_; }
	const World& World() const noexcept { return world_; }
	const WorldRenderer& WorldRenderer() const noexcept { return world_renderer_; }
};

#endif // ENGINE_HPP

