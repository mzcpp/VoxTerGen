#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "core/Engine.hpp"

struct SDL_Window;
struct TTF_Font;

struct ScreenDimensionsData
{
	int screen_width_ = 0;
	int screen_height_ = 0;
	float aspect_ratio_ = 0.0f;
};

/**
 * @brief Main application class.
 *
 * Manages the lifecycle of the application, including initialization,
 * the main loop, event handling, rendering, and shutdown.
 *
 * This class is responsible for:
 * - Initializing SDL and related subsystems
 * - Creating the SDL2 window and OpenGL context
 * - Running the main application loop
 */
class Application
{
private:
	bool initialized_image_;
	bool initialized_ttf_;
	bool initialized_mixer_;
	bool initialized_sdl_;

	SDL_Window* window_;
	TTF_Font* font_;

	SDL_GLContext gl_context_;

	ScreenDimensionsData screen_dimensions_data_;

	Engine engine_;

	bool running_;

public:
	Application();

	~Application();

	bool Initialize();

	void Run();

private:
	void HandleEvents();

	void Tick();

	void Render(float alpha);

	bool InitSDL();

	bool CreateWindow();

	bool CreateOpenGLContext();

	bool InitOpenGL();

	bool InitSDLModules();

	void Finalize();

	void GetHardwareInfo();

#ifdef _DEBUG
	void InitDebugGLCallback();
#endif

	bool IsSoftwareRenderer();
};

#endif // APPLICATION_HPP