#ifndef CAMERA_CONTROLLER_HPP
#define CAMERA_CONTROLLER_HPP

#include "input/InputManager.hpp"
#include "graphics/Camera.hpp"
#include "utils/Constants.hpp"

/**
 * @brief Handles camera movement, rotation, and zoom based on user input.
 *
 * The CameraController class updates a Camera instance according to keyboard and mouse input.
 * It provides methods to apply movement, rotation, and zoom in a consistent and frame-independent manner.
 */
class CameraController
{
private:
    Camera& camera_;
    float movement_speed_;
    float move_sensitivity_;
    float zoom_sensitivity_;

public:
    /**
     * @brief Constructs a CameraController for the given camera.
     *
     * Initializes movement speed, rotation sensitivity, and zoom sensitivity from constants.
     *
     * @param cam Reference to the Camera instance to control
     */
    CameraController(Camera& cam) :
        camera_(cam),
        movement_speed_(constants::camera::speed),
        move_sensitivity_(constants::camera::move_sensitivity),
        zoom_sensitivity_(constants::camera::zoom_sensitivity)
    {
    }

    CameraController(const CameraController& camera) = delete;
    CameraController& operator=(const CameraController&) = delete;

    CameraController(CameraController&& other) = delete;
    CameraController& operator=(CameraController&& other) = delete;

    /**
     * @brief Applies movement input (WASD keys) to the camera.
     *
     * Updates the camera position based on pressed keys and delta time.
     * Also updates the camera's simulation matrices and frustum planes.
     *
     * @param input The input manager providing keyboard/mouse input
     * @param delta_time Time elapsed since last frame, in seconds
     * @param aspect_ratio Aspect ratio of the viewport (width / height)
     */
    void ApplyInput(const InputManager& input, float delta_time, float aspect_ratio);

    /**
     * @brief Applies rotation input (mouse movement) to the camera.
     *
     * Updates the camera's yaw and pitch according to mouse delta and clamps pitch
     * to valid ranges. Updates camera vectors and frustum planes.
     *
     * @param input The input manager providing mouse movement
     */
    void ApplyRotation(const InputManager& input);

    /**
     * @brief Applies zoom input (mouse wheel) to the camera.
     *
     * Adjusts the camera's zoom (field of view) based on mouse wheel input, clamped
     * to valid zoom limits, and updates frustum planes.
     *
     * @param input The input manager providing mouse wheel input
     */
    void ApplyZoom(const InputManager& input);
};

#endif // CAMERA_CONTROLLER_HPP