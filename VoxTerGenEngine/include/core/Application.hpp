#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "input/InputManager.hpp"
#include "graphics/Camera.hpp"
#include "input/CameraController.hpp"

struct SDL_Window;
struct TTF_Font;

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

	int screen_width_;
	int screen_height_;
	float aspect_ratio_;

	InputManager input_manager_;
	Camera camera_;
	CameraController camera_controller_;

	bool running_;

public:
	Application();

	~Application();

	bool Initialize();

	void Run();

	void HandleEvents();

	void Tick();

	void Render(float alpha);

private:
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

#endif // GAME_HPP