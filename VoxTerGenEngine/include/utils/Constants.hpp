#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string_view>

/**
 * @brief Contains global constants used throughout the engine.
 */
namespace constants
{
    /**
     * @brief Window-related settings such as title and dimensions.
     */
    namespace window
    {
        inline constexpr std::string_view title = "VoxTerGen";
        inline constexpr int screen_width = 1960;
        inline constexpr int screen_height = 1280;
    } // namespace window

    /**
     * @brief OpenGL context and framebuffer settings.
     */
    namespace open_gl
    {
        inline constexpr int required_gl_major = 4;
        inline constexpr int required_gl_minor = 5;
        inline constexpr int double_buffer = 1;
        inline constexpr int depth_size = 24;
    } // namespace open_gl

    /**
     * @brief Audio system configuration.
     */
    namespace audio
    {
        inline constexpr int audio_freq = 44100;
        inline constexpr int channels = 2;
        inline constexpr int chunk_size = 2048;
    } // namespace audio

    /**
     * @brief Engine timing constants for tick rate and delta time.
     */
    namespace engine
    {
        inline constexpr double tick_rate = 60.0;
        inline constexpr double tick_dt = 1.0 / tick_rate;
    } // namespace engine

    /**
     * @brief Chunk-related data such as width, depth and height.
     */
    namespace chunk
    {
        inline constexpr int width = 16;
        inline constexpr int depth = 16;
        inline constexpr int height = 128;
        inline constexpr int size = width * depth * height;
        inline constexpr int default_radius = 0;
    } // namespace chunk

    /**
     * @brief Math-related constants.
     */
    namespace math
    {
        inline constexpr float float_epsilon = 1e-5f;
    } // namespace math

    /**
     * @brief Paths to shader resources.
     */
    namespace paths
    {
        inline constexpr const char* cube_vertex_shader = "res/gfx/shaders/cube_shader.vert";
        inline constexpr const char* cube_fragment_shader = "res/gfx/shaders/cube_shader.frag";
    } // namespace paths

    /**
     * @brief Camera-related constants for default orientation, movement, and frustum.
     */
    namespace camera
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
    } // namespace camera

    /**
     * @brief Texture-related constants such as atlas size, columns, and rows.
     */
    namespace texture
    {
        inline constexpr int tile_size = 16;
        inline constexpr int atlas_columns = 2;
        inline constexpr int atlas_rows = 4;
    } // namespace texture

} // namespace constants

#endif // CONSTANTS_HPP