#include "input/CameraController.hpp"
#include "input/InputManager.hpp"
#include "utils/MathUtils.hpp"
#include "graphics/Camera.hpp"

#include <glm/glm.hpp>

#include <SDL2/SDL.h>

#include <algorithm>

CameraController::CameraController(Camera& camera) :
    camera_(camera),
    movement_speed_(constants::camera::speed), 
    move_sensitivity_(constants::camera::move_sensitivity), 
    zoom_sensitivity_(constants::camera::zoom_sensitivity)
{   
}

void CameraController::ApplyInput(const InputManager& input, float delta_time, float aspect_ratio)
{
    if (!camera_.enabled_movement_)
    {
        return;
    }

    glm::vec3 move_dir(0.0f);

    if (input.KeyDown(SDL_SCANCODE_W))
    {
        move_dir += camera_.front_;
    }

    if (input.KeyDown(SDL_SCANCODE_S))
    {
        move_dir -= camera_.front_;
    }

    if (input.KeyDown(SDL_SCANCODE_A))
    {
        move_dir -= camera_.right_;
    }

    if (input.KeyDown(SDL_SCANCODE_D))
    {
        move_dir += camera_.right_;
    }

    if (input.KeyDown(SDL_SCANCODE_SPACE))
    {
        move_dir += camera_.world_up_;
    }

    if (input.KeyDown(SDL_SCANCODE_LCTRL))
    { 
        move_dir -= camera_.world_up_;
    }

    if (glm::length(move_dir) > 0.0f)
    {
        move_dir = glm::normalize(move_dir);

        camera_.position_ += move_dir * movement_speed_ * delta_time;

        camera_.changed_ = true;
        camera_.moving_ = true;
    }

    camera_.UpdateSimulationMatrices(aspect_ratio);
    camera_.UpdateFrustumPlanes();
}

void CameraController::ApplyRotation(const InputManager& input)
{
    if (!camera_.enabled_movement_)
    {
        return;
    }

    const glm::vec2 mouse_delta = input.MouseDelta();

    if (math_utils::FloatingPointNearZero(mouse_delta.x) && math_utils::FloatingPointNearZero(mouse_delta.y))
    {
        return;
    }

    camera_.yaw_ += mouse_delta.x * move_sensitivity_;
    camera_.pitch_ = std::clamp(camera_.pitch_ + (mouse_delta.y * move_sensitivity_), constants::camera::pitch_min, constants::camera::pitch_max);
    camera_.changed_ = true;
    camera_.moving_ = true;
    camera_.UpdateCameraVectors();
    camera_.UpdateFrustumPlanes();
}

void CameraController::ApplyZoom(const InputManager& input)
{
    if (!camera_.enabled_movement_ || math_utils::FloatingPointNearZero(input.MouseWheel()))
    {
        return;
    }

    camera_.zoom_ = std::clamp(camera_.zoom_ - (input.MouseWheel() * zoom_sensitivity_), constants::camera::zoom_min, constants::camera::zoom_max);
    camera_.changed_ = true;
    camera_.moving_ = true;
    camera_.UpdateFrustumPlanes();
}