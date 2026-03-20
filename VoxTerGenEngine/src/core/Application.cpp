#include "core/Application.hpp"
#include "utils/constants.hpp"
#include "utils/Logger.hpp"

#include <glad/glad/glad.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <algorithm>

Application::Application() :
	initialized_image_(false),
	initialized_ttf_(false),
	initialized_mixer_(false),
	initialized_sdl_(false),
	window_(nullptr),
	font_(nullptr),
	gl_context_(nullptr),
	screen_width_(constants::window::screen_width),
	screen_height_(constants::window::screen_height),
	aspect_ratio_(static_cast<float>(screen_width_) / static_cast<float>(screen_height_)),
	running_(false)
{
	Logger::Log(LogLevel::INFO, "Application object created.");
}

Application::~Application()
{
	Logger::Log(LogLevel::INFO, "Application object destroyed, finalizing...");
	Finalize();
}

bool Application::Initialize()
{
	if (!InitSDL())
	{
		return false;
	}

	if (!CreateWindow())
	{
		return false;
	}

	if (!CreateOpenGLContext())
	{
		return false;
	}

	if (!InitOpenGL())
	{
		return false;
	}

	if (!InitSDLModules())
	{
		return false;
	}

	return true;
}

void Application::Run()
{
	if (!Initialize())
	{
		Logger::Log(LogLevel::ERROR, "Game object failed to initialize!");
		return;
	}

	Logger::Log(LogLevel::INFO, "Game object initialized.");
	running_ = true;

	std::uint64_t last_time = SDL_GetPerformanceCounter();
	double delta = 0.0;

	int frames = 0;
	int ticks = 0;

	while (running_)
	{
		const std::uint64_t now = SDL_GetPerformanceCounter();
		const double elapsed = static_cast<double>(now - last_time) / static_cast<double>(SDL_GetPerformanceFrequency());

		last_time = now;
		delta += elapsed;

		HandleEvents();

		while (delta >= constants::engine::tick_dt)
		{
			Tick();
			delta -= constants::engine::tick_dt;
			++ticks;
		}

		const float alpha = std::clamp(static_cast<float>(delta / constants::engine::tick_dt), 0.0f, 1.0f);
		//printf("%Lf\n", alpha);
		Render(alpha);
		++frames;

		static double title_timer = 0.0;
		title_timer += elapsed;

		if (title_timer >= 1.0)
		{
			const double frame_ms = (1.0 / frames) * 1000.0; // average frame time over 1 second

			const std::string title =
				std::string(constants::window::title) +
				" | FPS: " + std::to_string(frames) +
				" | TPS: " + std::to_string(ticks) +
				" | Frame: " + std::to_string(frame_ms).substr(0, 5) + " ms";

			SDL_SetWindowTitle(window_, title.c_str());

			//printf("Frames: %d, Ticks: %d\n", frames, ticks);

			title_timer = 0.0;
			frames = 0;
			ticks = 0;
		}
	}
}

void Application::HandleEvents()
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
		{
			running_ = false;
			return;
		}

		if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				screen_width_ = e.window.data1;
				screen_height_ = e.window.data2;
				aspect_ratio_ = static_cast<float>(screen_width_) / static_cast<float>(screen_height_);

				glViewport(0, 0, screen_width_, screen_height_);

				// SetScreenWidth(screen_width_);
				// SetScreenHeight(screen_height_);

				Logger::Log(LogLevel::INFO, "Window resized to {}x{}", screen_width_, screen_height_);
			}
		}

		engine_.HandleEvents(e);
	}
}

void Application::Tick()
{
	engine_.Tick(aspect_ratio_);
}

void Application::Render(float alpha)
{
	engine_.Render(alpha);
	SDL_GL_SwapWindow(window_);
}

bool Application::InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		Logger::Log(LogLevel::ERROR, "SDL could not be initialized! SDL Error: {}", SDL_GetError());
		return false;
	}

	initialized_sdl_ = true;

	return true;
}

bool Application::CreateWindow()
{
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, constants::open_gl::required_gl_major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, constants::open_gl::required_gl_minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, constants::open_gl::double_buffer);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, constants::open_gl::depth_size);

#ifdef _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	window_ = SDL_CreateWindow(
		constants::window::title.data(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		constants::window::screen_width,
		constants::window::screen_height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
	);

	if (window_ == nullptr)
	{
		Logger::Log(LogLevel::ERROR, "Window could not be created! SDL Error: {}", SDL_GetError());
		return false;
	}

	if (engine_.Camera().EnabledMovement())
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	return true;
}

bool Application::CreateOpenGLContext()
{
	gl_context_ = SDL_GL_CreateContext(window_);

	if (gl_context_ == nullptr)
	{
		Logger::Log(LogLevel::ERROR, "OpenGL context could not be created: {}", SDL_GetError());
		return false;
	}

	SDL_GL_MakeCurrent(window_, gl_context_);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		Logger::Log(LogLevel::ERROR, "Failed to initialize GLAD");
		return false;
	}

	if (GLVersion.major < constants::open_gl::required_gl_major ||
		(GLVersion.major == constants::open_gl::required_gl_major &&
			GLVersion.minor < constants::open_gl::required_gl_minor))
	{
		Logger::Log(
			LogLevel::CRITICAL,
			"OpenGL {}.{} required, but {}.{} loaded",
			constants::open_gl::required_gl_major,
			constants::open_gl::required_gl_minor,
			GLVersion.major,
			GLVersion.minor
		);

		return false;
	}

	return true;
}

bool Application::InitOpenGL()
{
	const char* versionStr = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	if (!versionStr)
	{
		Logger::Log(LogLevel::CRITICAL, "Failed to query OpenGL version!");
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenGL Error", "Failed to query OpenGL version!\n ", window_);

		return false;
	}

	GLint major = 0;
	GLint minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	if (major < constants::open_gl::required_gl_major ||
		(major == constants::open_gl::required_gl_major && minor < constants::open_gl::required_gl_minor))
	{
		Logger::Log(
			LogLevel::CRITICAL,
			"OpenGL {}.{} required, but detected {}.{}!",
			constants::open_gl::required_gl_major,
			constants::open_gl::required_gl_minor,
			major,
			minor
		);

		const std::string msg = std::format(
			"OpenGL {}.{} or higher is required!\nPlease update your GPU drivers.",
			constants::open_gl::required_gl_major,
			constants::open_gl::required_gl_minor
		);

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenGL Version Error", msg.c_str(), window_);

		return false;
	}

#ifdef _DEBUG
	GetHardwareInfo();
#endif

	if (IsSoftwareRenderer())
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Renderer Error",
			"Your system is using a software OpenGL renderer.\n"
			"Install or update your GPU drivers.", 
			window_
		);

		return false;
	}

#ifdef _DEBUG
	InitDebugGLCallback();
#endif

	glViewport(0, 0, constants::window::screen_width, constants::window::screen_height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LESS);

	// V-Sync
	SDL_GL_SetSwapInterval(0);

	return true;
}

bool Application::InitSDLModules()
{
	constexpr int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;

	if (!(IMG_Init(img_flags) & img_flags))
	{
		Logger::Log(LogLevel::ERROR, "SDL_image could not be initialized! SDL_image Error: {}", IMG_GetError());
		return false;
	}

	initialized_image_ = true;

	if (TTF_Init() == -1)
	{
		Logger::Log(LogLevel::ERROR, "SDL_ttf could not be initialized! SDL_ttf Error: {}", TTF_GetError());
		return false;
	}

	initialized_ttf_ = true;

	if (Mix_OpenAudio(constants::audio::audio_freq, MIX_DEFAULT_FORMAT, constants::audio::channels, constants::audio::chunk_size) < 0)
	{
		Logger::Log(LogLevel::ERROR, "SDL_mixer could not be initialized! SDL_mixer Error: {}", Mix_GetError());
		return false;
	}

	initialized_mixer_ = true;

	return true;
}

void Application::Finalize()
{
	if (gl_context_ != nullptr)
	{
		SDL_GL_DeleteContext(gl_context_);
		gl_context_ = nullptr;
		Logger::Log(LogLevel::INFO, "OpenGL context deleted.");
	}

	if (initialized_mixer_)
	{
		Mix_CloseAudio();
		Mix_Quit();
		initialized_mixer_ = false;
		Logger::Log(LogLevel::INFO, "SDL_mixer closed.");
	}

	if (font_ != nullptr)
	{
		TTF_CloseFont(font_);
		font_ = nullptr;
		Logger::Log(LogLevel::INFO, "TTF font closed.");
	}

	if (initialized_ttf_)
	{
		TTF_Quit();
		initialized_ttf_ = false;
		Logger::Log(LogLevel::INFO, "SDL_ttf quit.");
	}

	if (initialized_image_)
	{
		IMG_Quit();
		initialized_image_ = false;
		Logger::Log(LogLevel::INFO, "SDL_image quit.");
	}

	if (window_ != nullptr)
	{
		SDL_DestroyWindow(window_);
		window_ = nullptr;
		Logger::Log(LogLevel::INFO, "SDL window destroyed.");
	}

	if (initialized_sdl_)
	{
		SDL_Quit();
		initialized_sdl_ = false;
		Logger::Log(LogLevel::INFO, "SDL quit.");
	}

	Logger::Log(LogLevel::INFO, "Finalize complete.");
}

void Application::GetHardwareInfo()
{
	Logger::Log(LogLevel::INFO, "==================== System Info ===================");

	const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	const char* shading_version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

	Logger::Log(LogLevel::INFO, "CPU Cores: {}", SDL_GetCPUCount());
	Logger::Log(LogLevel::INFO, "System RAM: {} MB", SDL_GetSystemRAM());
	Logger::Log(LogLevel::INFO, "GPU Vendor: {}", vendor ? vendor : "Unknown");
	Logger::Log(LogLevel::INFO, "GPU Renderer: {}", renderer ? renderer : "Unknown");
	Logger::Log(LogLevel::INFO, "OpenGL Version: {}", version ? version : "Unknown");
	Logger::Log(LogLevel::INFO, "OpenGL Shading Language Version: {}", shading_version ? shading_version : "Unknown");
	Logger::Log(LogLevel::INFO, "=================================================");
}

#ifdef _DEBUG
void Application::InitDebugGLCallback()
{
	if (GLAD_GL_VERSION_4_3)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glDebugMessageCallback(
			[](GLenum source, GLenum type, GLuint id, GLenum severity,
				GLsizei length, const GLchar* message, const void* userParam)
			{
				(void)source;
				(void)type;
				(void)userParam;
				std::string msg(message, length);

				LogLevel level = LogLevel::INFO;

				switch (severity)
				{
				case GL_DEBUG_SEVERITY_HIGH: level = LogLevel::CRITICAL; break;
				case GL_DEBUG_SEVERITY_MEDIUM: level = LogLevel::ERROR; break;
				case GL_DEBUG_SEVERITY_LOW: level = LogLevel::WARNING; break;
				case GL_DEBUG_SEVERITY_NOTIFICATION: level = LogLevel::INFO; break;
				}

				Logger::Log(level, "OpenGL Debug [{}]: {}", id, msg);
			},
			nullptr
		);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}
#endif

bool Application::IsSoftwareRenderer()
{
	const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

	if (!renderer)
	{
		return true;
	}

	std::string r(renderer);

	for (char& c : r)
	{
		c = static_cast<char>(std::tolower(c));
	}

	static const std::vector<std::string> software_indicators =
	{
		"llvmpipe",		// Mesa CPU rasterizer
		"softpipe",		// Older Mesa software
		"software",		// Generic
		"swiftshader",	// Google SwiftShader
		"virtio",		// Virtualized GPU fallback
	};

	for (const auto& keyword : software_indicators)
	{
		if (r.find(keyword) != std::string::npos)
		{
			return true;
		}
	}

	return false;
}