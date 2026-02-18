#include "input/CameraController.hpp"
#include "input/InputManager.hpp"
#include "utils/MathUtils.hpp"

#include <SDL2/SDL.h>

#include <algorithm>

void CameraController::ApplyInput(const InputManager& input, float delta_time, float aspect_ratio)
{
    if (!camera_.enabled_movement_)
    {
        return;
    }

    if (input.KeyDown(SDL_SCANCODE_W))
    {
        camera_.position_ += camera_.front_ * movement_speed_ * delta_time;
        camera_.changed_ = true;
        camera_.moving_ = true;
    }
    
    if (input.KeyDown(SDL_SCANCODE_S))
    {
        camera_.position_ -= camera_.front_ * movement_speed_ * delta_time;
        camera_.changed_ = true;
        camera_.moving_ = true;
    }

    if (input.KeyDown(SDL_SCANCODE_A))
    {
        camera_.position_ -= camera_.right_ * movement_speed_ * delta_time;
        camera_.changed_ = true;
        camera_.moving_ = true;
    }

    if (input.KeyDown(SDL_SCANCODE_D))
    {
        camera_.position_ += camera_.right_ * movement_speed_ * delta_time;
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

    glm::vec2 mouse_delta = input.MouseDelta();

    if (FloatingPointNearZero(mouse_delta.x) && FloatingPointNearZero(mouse_delta.y))
    {
        return;
    }

    camera_.yaw_ += mouse_delta.x * move_sensitivity_;
    camera_.pitch_ += mouse_delta.y * move_sensitivity_;

    camera_.pitch_ = std::clamp
    (
        camera_.pitch_, 
        constants::camera::pitch_min,
        constants::camera::pitch_max
    );
    
    camera_.changed_ = true;
    camera_.moving_ = true;
    camera_.UpdateCameraVectors();
    camera_.UpdateFrustumPlanes();
}

void CameraController::ApplyZoom(const InputManager& input)
{
    if (!camera_.enabled_movement_ || FloatingPointNearZero(input.MouseWheel()))
    {
        return;
    }

    camera_.zoom_ = std::clamp
    (
        camera_.zoom_ - (input.MouseWheel() * zoom_sensitivity_), 
        constants::camera::zoom_min,
        constants::camera::zoom_max
    );

    camera_.changed_ = true;
    camera_.moving_ = true;
    camera_.UpdateFrustumPlanes();
}