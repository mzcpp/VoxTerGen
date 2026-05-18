#include "input/ObserverController.hpp"
#include "input/InputManager.hpp"
#include "graphics/Camera.hpp"
#include "utils/Constants.hpp"
#include "utils/MathUtils.hpp"

#include <glm/glm.hpp>

#include <SDL2/SDL.h>

#include <algorithm>

ObserverController::ObserverController(Observer& observer) : observer_(observer)
{
}

void ObserverController::Tick(const InputManager& input, Camera& camera)
{
    ApplyKeyboardInput(input, camera);
    ApplyMouseRotation(input, camera);
}

void ObserverController::ApplyKeyboardInput(const InputManager& input, Camera& camera)
{
    observer_.prev_position_ = observer_.position_;

    if (!camera.EnabledMovement())
    {
        camera.SetPrevPos(observer_.prev_position_);
    }

    glm::vec3 move_dir(0.0f);

    if (input.KeyDown(SDL_SCANCODE_W))
    {
       move_dir += observer_.Front();
    }

    if (input.KeyDown(SDL_SCANCODE_S))
    {
       move_dir -= observer_.Front();
    }

    if (input.KeyDown(SDL_SCANCODE_A))
    {
       move_dir -= observer_.Right();
    }

    if (input.KeyDown(SDL_SCANCODE_D))
    {
       move_dir += observer_.Right();
    }

    if (input.KeyDown(SDL_SCANCODE_SPACE))
    {
       move_dir += constants::math::world_up;
    }

    if (input.KeyDown(SDL_SCANCODE_LCTRL))
    {
       move_dir -= constants::math::world_up;
    }

    if (glm::length(move_dir) > 0.0f)
    {
       move_dir = glm::normalize(move_dir);

       observer_.position_ += move_dir * constants::observer::movement_speed * static_cast<float>(constants::engine::tick_dt);
       observer_.moving_ = true;
      
       if (!camera.EnabledMovement())
       {
           camera.SetPos(observer_.position_);
           camera.SetMoving(true);
       }
    }
}

void ObserverController::ApplyMouseRotation(const InputManager& input, Camera& camera)
{
    observer_.prev_yaw_ = observer_.yaw_;
    observer_.prev_pitch_ = observer_.pitch_;

    if (!camera.EnabledRotation())
    {
        camera.SetPrevYaw(observer_.prev_yaw_);
        camera.SetPrevPitch(observer_.prev_pitch_);
    }

    const glm::vec2 mouse_delta = input.MouseDelta();

    if (math_utils::FloatingPointNearZero(mouse_delta.x) && math_utils::FloatingPointNearZero(mouse_delta.y))
    {
        return;
    }

    observer_.yaw_ += mouse_delta.x * constants::camera::move_sensitivity;
    observer_.pitch_ = std::clamp(observer_.pitch_ + (mouse_delta.y * constants::camera::move_sensitivity), constants::camera::pitch_min, constants::camera::pitch_max);
    observer_.moving_ = true;

    if (!camera.EnabledRotation())
    {
        camera.SetYaw(observer_.yaw_);
        camera.SetPitch(observer_.pitch_);
        camera.SetMoving(true);
    }

    // more TODO later (WoW style camera) 
}