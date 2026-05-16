#include "core/Engine.hpp"

#include <glad/glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <SDL2/SDL.h>

Engine::Engine() : camera_controller_(camera_), observer_controller_(observer_)
{
}

void Engine::Initialize()
{
	resource_manager_.InitializeResources();
	
	// TODO
	//observer_.SetPosition(CalculateObserverPosition());
	
	world_.InitChunks(constants::chunk::default_radius);
}

void Engine::HandleEvents(SDL_Event e)
{
	input_manager_.ProcessEvent(e);
}

void Engine::Tick(float aspect_ratio)
{
	camera_controller_.Tick(input_manager_);
	camera_.Tick(aspect_ratio);

	observer_controller_.Tick(input_manager_, camera_);
	observer_.Tick();

	world_.Tick(chunk_event_queue_, camera_);
	world_renderer_.Tick(chunk_event_queue_);
	
	input_manager_.ResetFrameState();
}

void Engine::Render(float alpha)
{
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const glm::mat4 interpolated_view = camera_.InterpolatedViewMatrix(alpha);
	const glm::mat4 proj = camera_.ProjectionMatrix();

	world_renderer_.RenderWorld(interpolated_view, proj, resource_manager_);
}

glm::dvec3 Engine::CalculateObserverPosition() const
{
	// TODO:
	return glm::dvec3(0.0);
}