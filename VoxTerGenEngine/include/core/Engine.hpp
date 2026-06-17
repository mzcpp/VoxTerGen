#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "core/ResourceManager.hpp"
#include "core/Settings.hpp"

#include "graphics/Camera.hpp"

#include "input/CameraController.hpp"
#include "input/InputManager.hpp"
#include "input/ObserverController.hpp"

#include "physics/CollisionSystem.hpp"

#include "render/WorldRenderer.hpp"

#include "world/Observer.hpp"
#include "world/World.hpp"

#include <queue>

union SDL_Event;

class Engine
{
private:
	InputManager input_manager_;
	Camera camera_;
	Observer observer_;
	CameraController camera_controller_;
	ObserverController observer_controller_;
	ResourceManager resource_manager_;
	Settings settings_;
	World world_;
	WorldRenderer world_renderer_;
	CollisionSystem collision_system_;

	std::queue<ChunkEvent> chunk_event_queue_;

public:
	Engine();

	void Initialize();

	void BeginFrame();

	void ApplyInput();

	void HandleEvents(SDL_Event e);

	void Tick(float aspect_ratio);

	void Render(float alpha);

	// Getters
	const InputManager& InputManager() const noexcept { return input_manager_; }
	const Camera& Camera() const noexcept { return camera_; }
	const CameraController& CameraController() const noexcept { return camera_controller_; }
	const Observer& Observer() const noexcept { return observer_; }
	const ObserverController& ObserverController() const noexcept { return observer_controller_; }
	const ResourceManager& ResourceManager() const noexcept { return resource_manager_; }
	const Settings& Settings() const noexcept { return settings_; }
	const World& World() const noexcept { return world_; }
	const WorldRenderer& WorldRenderer() const noexcept { return world_renderer_; }

private:
	glm::dvec3 CalculateObserverPosition() const;
};

#endif // ENGINE_HPP

