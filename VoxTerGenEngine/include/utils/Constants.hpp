#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <glm/vec3.hpp>

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
        inline constexpr int default_radius = 1;
    } // namespace chunk

    /**
     * @brief Math-related constants.
     */
    namespace math
    {
        inline constexpr float float_abs_epsilon = 1e-6f;
        inline constexpr float float_rel_epsilon = 1e-5f;
        inline constexpr double double_abs_epsilon = 1e-12;
        inline constexpr double double_rel_epsilon = 1e-9;
        inline constexpr glm::vec3 world_up = { 0.0f, 1.0f, 0.0f };
    } // namespace math

    namespace geometry
    {
        inline constexpr glm::vec3 world_up = { 0.0f, 1.0f, 0.0f };
        inline constexpr glm::dvec3 spawn_point = { 0.0, 5.0, 0.0 };
    } // namespace geometry

    /**
     * @brief Physics-related constants.
     */
    namespace physics
    {
        inline constexpr double gravity = -9.81;
        inline constexpr double collision_epsilon = 1e-8;
    } // namespace physics

    /**
     * @brief Paths to resources.
     */
    namespace paths
    {
        inline constexpr const char* texture_atlas = "res/gfx/textures/atlas.png";
        inline constexpr const char* sky_cubemap = "res/gfx/textures/cloudbox.png";
        inline constexpr const char* chunk_mesh_vertex_shader = "res/gfx/shaders/chunk_mesh.vert";
        inline constexpr const char* chunk_mesh_fragment_shader = "res/gfx/shaders/chunk_mesh.frag";
        inline constexpr const char* default_font = "res/font/VT323-Regular.ttf";
        inline constexpr const char* button_click = "res/sfx/click.wav";
    } // namespace paths

    /**
     * @brief Camera-related constants for default orientation, movement, yaw, pitch and frustum.
     */
    namespace camera
    {
        inline constexpr float yaw = -90.0f;
        inline constexpr float pitch = 0.0f;
        inline constexpr float pitch_min = -89.0f;
        inline constexpr float pitch_max = 89.0f;
        inline constexpr double movement_speed = 10.0;
        inline constexpr float move_sensitivity = 0.1f;
        inline constexpr float zoom_sensitivity = 1.0f;
        inline constexpr float zoom = 45.0f;
        inline constexpr float zoom_min = 1.0f;
        inline constexpr float zoom_max = 45.0f;
        inline constexpr float near_plane = 0.1f;
        inline constexpr float far_plane = 450.0f;
    } // namespace camera

    /**
     * @brief Observer-related constants for default orientation, movement, yaw, and pitch.
     */
    namespace observer
    {
        inline constexpr float yaw = -90.0f;
        inline constexpr float pitch = 0.0f;
        inline constexpr float pitch_min = -89.0f;
        inline constexpr float pitch_max = 89.0f;
        inline constexpr double movement_speed = 5.0;
        inline constexpr float move_sensitivity = 0.1f;
        inline constexpr double width = 0.6;
        inline constexpr double height = 1.8;
        inline constexpr double depth = 0.6;
        inline constexpr glm::dvec3 pos_offset = {
            constants::observer::width / 2.0,
            constants::observer::height / 4.0,
            constants::observer::depth / 2.0
        };
    } // namespace observer

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