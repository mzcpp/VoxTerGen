#ifndef CAMERA_CONTROLLER_HPP
#define CAMERA_CONTROLLER_HPP

#include "utils/Constants.hpp"

class Camera;
class InputManager;

class CameraController
{
private:
    Camera& camera_;
    float movement_speed_;
    float move_sensitivity_;
    float zoom_sensitivity_;

public:
    CameraController(Camera& cam);

    CameraController(const CameraController& other) = delete;
    CameraController& operator=(const CameraController& other) = delete;

    CameraController(CameraController&& other) = delete;
    CameraController& operator=(CameraController&& other) = delete;

    void ApplyInput(const InputManager& input, float delta_time, float aspect_ratio);

    void ApplyRotation(const InputManager& input);

    void ApplyZoom(const InputManager& input);
};

#endif // CAMERA_CONTROLLER_HPP