#ifndef CAMERA_CONTROLLER_HPP
#define CAMERA_CONTROLLER_HPP

#include "input/InputManager.hpp"
#include "graphics/Camera.hpp"
#include "utils/Constants.hpp"

class CameraController
{
private:
    Camera& camera_;
    float movement_speed_;
    float move_sensitivity_;
    float zoom_sensitivity_;

public:
    CameraController(Camera& cam) : 
        camera_(cam), 
        movement_speed_(Constants::Camera::speed),
        move_sensitivity_(Constants::Camera::move_sensitivity), 
        zoom_sensitivity_(Constants::Camera::zoom_sensitivity)
    {}

    CameraController(const CameraController& camera) = delete;
    CameraController& operator=(const CameraController&) = delete;

    CameraController(CameraController&& other) = delete;
    CameraController& operator=(CameraController&& other) = delete;

    void ApplyInput(const InputManager& input, float delta_time, float aspect_ratio);
    
    void ApplyRotation(const InputManager& input);

    void ApplyZoom(const InputManager& input);
};

#endif

