#include "core/Engine.hpp"

#include <glad/glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <SDL2/SDL.h>

Engine::Engine() : camera_controller_(camera_)
{
}

void Engine::Initialize()
{
	resource_manager_.InitializeResources();
	// TODO: Make World::InitChunks and call InitChunks inside there too to make this prettier.
	world_.ChunkManagerRef().InitChunks(constants::chunk::default_radius);
}

void Engine::HandleEvents(SDL_Event e)
{
	input_manager_.ResetFrameState();
	input_manager_.ProcessEvent(e);

	if (e.type == SDL_MOUSEMOTION)
	{
		camera_controller_.ApplyRotation(input_manager_);
	}

	if (e.type == SDL_MOUSEWHEEL)
	{
		camera_controller_.ApplyZoom(input_manager_);
	}

	//if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f)
	//{
	//	for (auto& [world_coords, chunk] : world_.ChunkManagerRef().Chunks())
	//	{
	//		std::cout << "invalidated mesh!\n";
	//		chunk->SetMeshValid(false);
	//		world_.ChunkManagerRef().PushChunkIntoQueue(chunk.get());
	//	}
	//	//world_.ChunkManagerRef().GetChunkAt({ 0, 0 })->SetMeshValid(false);
	//}
}

void Engine::Tick(float aspect_ratio)
{
	camera_.PreTick();
	camera_controller_.ApplyInput(input_manager_, static_cast<float>(constants::engine::tick_dt), aspect_ratio);
	camera_.Tick(aspect_ratio);

	world_.Tick(chunk_event_queue_, camera_);
}

void Engine::Render(float alpha)
{
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const glm::mat4 interpolated_view = camera_.InterpolatedViewMatrix(alpha);
	const glm::mat4 proj = camera_.ProjectionMatrix();

	world_renderer_.RenderWorld(chunk_event_queue_, world_, interpolated_view, proj, resource_manager_);
	
	camera_.EndTick();
}