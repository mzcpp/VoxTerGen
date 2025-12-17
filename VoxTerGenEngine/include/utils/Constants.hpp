#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string_view>

namespace Constants
{
	namespace Window
	{
		inline constexpr std::string_view title = "VoxTerGen";
		inline constexpr int screen_width = 1960;
		inline constexpr int screen_height = 1280;
	} // namespace Window

	namespace OpenGL
	{
		inline constexpr int required_gl_major = 4;
		inline constexpr int required_gl_minor = 6;
		inline constexpr int double_buffer = 1;
		inline constexpr int depth_size = 24;
	} // namespace OpenGL

	namespace Audio
	{
		inline constexpr int audio_freq = 44100;
		inline constexpr int channels = 2;
		inline constexpr int chunk_size = 2048;
	} // namespace Audio

	namespace Engine
	{
		inline constexpr double tick_rate = 60.0;
		inline constexpr double tick_dt = 1.0 / tick_rate;
	} // namespace Engine

	namespace Math
	{
		inline constexpr float float_epsilon = 1e-5f;
	} // namespace Math

	namespace Paths
	{
		inline constexpr const char* cube_vertex_shader = "res/gfx/shaders/cube_shader.vert";
		inline constexpr const char* cube_fragment_shader = "res/gfx/shaders/cube_shader.frag";
	} // namespace Paths

	namespace Camera
	{
		inline constexpr float yaw = -46.0f;
		inline constexpr float pitch = -20.0f;
		inline constexpr float pitch_min = -89.0f;
		inline constexpr float pitch_max = 89.0f;
		inline constexpr float speed = 5.5f;
		inline constexpr float move_sensitivity = 0.1f;
		inline constexpr float zoom_sensitivity = 1.0f;
		inline constexpr float zoom = 45.0f;
		inline constexpr float zoom_min = 1.0f;
		inline constexpr float zoom_max = 45.0f;
		inline constexpr float near_plane = 0.1f;
		inline constexpr float far_plane = 150.0f;
	} // namespace Camera
} // namespace Constants

#endif