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
    CameraController(Camera& camera);

    CameraController(const CameraController& other) = delete;
    CameraController& operator=(const CameraController& other) = delete;

    CameraController(CameraController&& other) = delete;
    CameraController& operator=(CameraController&& other) = delete;

    void GatherInput(const InputManager& input_manager);

    void Tick(const InputManager& input_manager);

    void ApplyChanges();

    void ApplyKeyboardInput(const InputManager& input_manager);

    void ApplyMouseRotation(const InputManager& input_manager);

    void ApplyZoom(const InputManager& input_manager);
};

#endif // CAMERA_CONTROLLER_HPP