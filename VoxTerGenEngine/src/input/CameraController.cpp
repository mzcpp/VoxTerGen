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

void CameraController::Tick(const InputManager& input_manager)
{
    ApplyRotation(input_manager);
    ApplyZoom(input_manager);
    ApplyInput(input_manager);
}

void CameraController::ApplyInput(const InputManager& input_manager)
{
    if (!camera_.enabled_movement_)
    {
        return;
    }

    camera_.prev_position_ = camera_.position_;

    glm::vec3 move_dir(0.0f);

    if (input_manager.KeyDown(SDL_SCANCODE_W))
    {
        move_dir += camera_.front_;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_S))
    {
        move_dir -= camera_.front_;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_A))
    {
        move_dir -= camera_.right_;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_D))
    {
        move_dir += camera_.right_;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_SPACE))
    {
        move_dir += camera_.world_up_;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_LCTRL))
    { 
        move_dir -= camera_.world_up_;
    }


    if (glm::length(move_dir) > 0.0f)
    {
        move_dir = glm::normalize(move_dir);
        
        camera_.position_ += move_dir * movement_speed_ * static_cast<float>(constants::engine::tick_dt);
        camera_.stale_ = true;
        camera_.moving_ = true;
    }
}

void CameraController::ApplyRotation(const InputManager& input_manager)
{
    if (!camera_.enabled_movement_)
    {
        return;
    }

    camera_.prev_yaw_ = camera_.yaw_;
    camera_.prev_pitch_ = camera_.pitch_;

    const glm::vec2 mouse_delta = input_manager.MouseDelta();

    if (math_utils::FloatingPointNearZero(mouse_delta.x) && math_utils::FloatingPointNearZero(mouse_delta.y))
    {
        return;
    }

    camera_.yaw_ += mouse_delta.x * move_sensitivity_;
    camera_.pitch_ = std::clamp(camera_.pitch_ + (mouse_delta.y * move_sensitivity_), constants::camera::pitch_min, constants::camera::pitch_max);
    camera_.stale_ = true;
    camera_.moving_ = true;
}

void CameraController::ApplyZoom(const InputManager& input_manager)
{
    if (!camera_.enabled_movement_ || math_utils::FloatingPointNearZero(input_manager.MouseWheel()))
    {
        return;
    }

    camera_.zoom_ = std::clamp(camera_.zoom_ - (input_manager.MouseWheel() * zoom_sensitivity_), constants::camera::zoom_min, constants::camera::zoom_max);
    camera_.stale_ = true;
    camera_.moving_ = true;
}