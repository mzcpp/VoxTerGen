#include "core/Application.hpp"

#include "core/Engine.hpp"

#include "threading/ThreadPool.hpp"

#include <glad/glad/glad.h>

#include <SDL2/SDL.h>

#include <thread>

Engine::Engine(const ScreenDimensionsData& screen_dimensions_data) : 
	screen_dimensions_data_(screen_dimensions_data), 
	thread_pool_(std::max(1u, std::thread::hardware_concurrency() - 1)), 
	camera_controller_(camera_), 
	observer_controller_(observer_), 
	world_(observer_, thread_pool_), 
	world_renderer_(screen_dimensions_data_, camera_)
{
}

void Engine::Initialize()
{
	resource_manager_.InitializeResources();
	
	// TODO
	//observer_.SetPosition(CalculateObserverPosition());
	
	world_.InitChunks(constants::chunk::default_radius);
	world_renderer_.Initialize();
}

void Engine::BeginFrame()
{
	input_manager_.ResetFrameState();
}

void Engine::GatherInput()
{
	observer_controller_.GatherInput(input_manager_);
	camera_controller_.GatherInput(input_manager_);
}

void Engine::ApplyInput(double frame_dt)
{
	observer_controller_.ApplyChanges(camera_);
	camera_controller_.ApplyChanges(frame_dt);
}

void Engine::HandleEvents(SDL_Event e)
{
	input_manager_.ProcessEvent(e);
}

void Engine::Tick()
{
	const ChunkManager& chunk_manager = world_.ChunkManagerRef();
	
	observer_controller_.Tick(collision_system_, chunk_manager, camera_);
	camera_controller_.Tick(chunk_manager);
	
	observer_.Tick();
	camera_.Tick(screen_dimensions_data_.aspect_ratio_);

	world_.Tick(chunk_event_queue_);
	world_renderer_.Tick(chunk_event_queue_);
}

void Engine::Render(float alpha)
{
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	world_renderer_.RenderWorld(alpha, resource_manager_);
}