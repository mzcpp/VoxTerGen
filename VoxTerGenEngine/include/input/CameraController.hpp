#ifndef CAMERA_CONTROLLER_HPP
#define CAMERA_CONTROLLER_HPP

#include "utils/Constants.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Camera;
class InputManager;

class CameraController
{
private:
    Camera& camera_;
    double movement_speed_;
    float move_sensitivity_;
    float zoom_sensitivity_;

    glm::dvec3 move_dir_;
    glm::vec2 mouse_delta_;
    float mouse_wheel_;

public:
    CameraController(Camera& camera);

    CameraController(const CameraController& other) = delete;
    CameraController& operator=(const CameraController& other) = delete;

    CameraController(CameraController&& other) = delete;
    CameraController& operator=(CameraController&& other) = delete;

    void GatherInput(const InputManager& input_manager);

    void Tick(const ChunkManager& chunk_manager);

    void ApplyChanges(double frame_dt);

    void ApplyKeyboardInput(double frame_dt);

    void ApplyMouseRotation();

    void ApplyZoom();
};

#endif // CAMERA_CONTROLLER_HPP