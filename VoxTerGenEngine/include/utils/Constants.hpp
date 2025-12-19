#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string_view>

/**
 * @brief Contains global constants used throughout the engine.
 */
namespace Constants
{
    /**
     * @brief Window-related settings such as title and dimensions.
     */
    namespace Window
    {
        inline constexpr std::string_view title = "VoxTerGen";
        inline constexpr int screen_width = 1960;
        inline constexpr int screen_height = 1280;
    } // namespace Window

    /**
     * @brief OpenGL context and framebuffer settings.
     */
    namespace OpenGL
    {
        inline constexpr int required_gl_major = 4;
        inline constexpr int required_gl_minor = 6;
        inline constexpr int double_buffer = 1;
        inline constexpr int depth_size = 24;
    } // namespace OpenGL

    /**
     * @brief Audio system configuration.
     */
    namespace Audio
    {
        inline constexpr int audio_freq = 44100;
        inline constexpr int channels = 2;
        inline constexpr int chunk_size = 2048;
    } // namespace Audio

    /**
     * @brief Engine timing constants for tick rate and delta time.
     */
    namespace Engine
    {
        inline constexpr double tick_rate = 60.0;
        inline constexpr double tick_dt = 1.0 / tick_rate;
    } // namespace Engine

    /**
     * @brief Chunk-related data such as width, depth and height.
     */
    namespace Chunk
    {
        inline constexpr int width = 16;
        inline constexpr int depth = 16;
        inline constexpr int height = 128;
    } // namespace Chunk

    /**
     * @brief Math-related constants.
     */
    namespace Math
    {
        inline constexpr float float_epsilon = 1e-5f;
    } // namespace Math

    /**
     * @brief Paths to commonly used shader resources.
     */
    namespace Paths
    {
        inline constexpr const char* cube_vertex_shader = "res/gfx/shaders/cube_shader.vert";
        inline constexpr const char* cube_fragment_shader = "res/gfx/shaders/cube_shader.frag";
    } // namespace Paths

    /**
     * @brief Camera-related constants for default orientation, movement, and frustum.
     */
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

#endif // CONSTANTS_HPP