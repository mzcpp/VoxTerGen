#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "input/InputManager.hpp"
#include "graphics/Camera.hpp"
#include "input/CameraController.hpp"
#include "core/Engine.hpp"

struct SDL_Window;
struct TTF_Font;

/**
 * @brief Main application class.
 *
 * Manages the lifecycle of the application, including initialization,
 * the main loop, event handling, rendering, and shutdown.
 *
 * This class is responsible for:
 * - Initializing SDL and related subsystems
 * - Creating the SDL2 window and OpenGL context
 * - Managing input, camera, and rendering
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

	int screen_width_;
	int screen_height_;
	float aspect_ratio_;

	InputManager input_manager_;
	Camera camera_;
	CameraController camera_controller_;

	Engine engine_;

	bool running_;

public:
	/**
	 * @brief Constructs the Application.
	 *
	 * Initializes member variables to default values.
	 * No SDL or OpenGL initialization is performed here.
	 */
	Application();

	/**
	 * @brief Destroys the Application.
	 *
	 * Ensures that all resources are properly released
	 * and subsystems are shut down.
	 */
	~Application();

	/**
	 * @brief Initializes the application.
	 *
	 * Initializes SDL, creates the window and OpenGL context,
	 * initializes required SDL modules, and prepares the application
	 * for execution.
	 *
	 * @return True if initialization succeeded, false otherwise.
	 */
	bool Initialize();

	/**
	 * @brief Runs the main application loop.
	 *
	 * Processes events, updates application state, and renders
	 * frames until the application exits.
	 */
	void Run();

private:
	/**
	 * @brief Handles input and window events.
	 *
	 * Processes SDL events such as keyboard, mouse, and window events.
	 */
	void HandleEvents();

	/**
	 * @brief Updates application logic.
	 *
	 * Advances the simulation state for the current frame.
	 */
	void Tick();

	/**
	 * @brief Renders the current frame.
	 *
	 * @param alpha Interpolation factor between simulation steps.
	 */
	void Render(float alpha);

	/**
	 * @brief Initializes the core SDL subsystem.
	 *
	 * @return True if SDL initialization succeeded, false otherwise.
	 */
	bool InitSDL();

	/**
	 * @brief Creates the application window.
	 *
	 * @return True if the window was created successfully, false otherwise.
	 */
	bool CreateWindow();

	/**
	 * @brief Creates the OpenGL context.
	 *
	 * @return True if the OpenGL context was created successfully, false otherwise.
	 */
	bool CreateOpenGLContext();

	/**
	 * @brief Initializes OpenGL state.
	 *
	 * Sets up OpenGL settings required for rendering.
	 *
	 * @return True if OpenGL initialization succeeded, false otherwise.
	 */
	bool InitOpenGL();

	/**
	 * @brief Initializes additional SDL modules.
	 *
	 * Initializes SDL_image, SDL_ttf, SDL_mixer, or other
	 * optional SDL subsystems as required.
	 *
	 * @return True if all modules were initialized successfully, false otherwise.
	 */
	bool InitSDLModules();

	/**
	 * @brief Shuts down the application and releases resources.
	 *
	 * Cleans up the OpenGL context, destroys the window,
	 * and shuts down all initialized SDL subsystems.
	 */
	void Finalize();

	/**
	 * @brief Queries and logs hardware information.
	 *
	 * Retrieves information such as renderer type, GPU,
	 * and other system capabilities.
	 */
	void GetHardwareInfo();

#ifdef _DEBUG
	/**
	 * @brief Initializes the OpenGL debug callback.
	 *
	 * Enables OpenGL debug output when running in debug builds.
	 */
	void InitDebugGLCallback();
#endif

	/**
	 * @brief Checks whether a software renderer is being used.
	 *
	 * @return True if a software renderer is active, false otherwise.
	 */
	bool IsSoftwareRenderer();
};

#endif // APPLICATION_HPP