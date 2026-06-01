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

void ObserverController::Tick(glm::vec3 movement_vector, glm::vec2 mouse_delta, Camera& camera)
{
    ApplyMovementVector(movement_vector, camera);
    ApplyMouseRotation(mouse_delta, camera);
}

glm::vec3 ObserverController::GetMovementVector(const InputManager& input) const
{
    glm::vec3 move_vec(0.0f);

    if (input.KeyDown(SDL_SCANCODE_W))
    {
       move_vec += observer_.Front();
    }

    if (input.KeyDown(SDL_SCANCODE_S))
    {
       move_vec -= observer_.Front();
    }

    if (input.KeyDown(SDL_SCANCODE_A))
    {
       move_vec -= observer_.Right();
    }

    if (input.KeyDown(SDL_SCANCODE_D))
    {
       move_vec += observer_.Right();
    }

    if (input.KeyDown(SDL_SCANCODE_SPACE))
    {
       move_vec += constants::math::world_up;
    }

    if (input.KeyDown(SDL_SCANCODE_LCTRL))
    {
       move_vec -= constants::math::world_up;
    }

    if (glm::length(move_vec) > 0.0f)
    {
       move_vec = glm::normalize(move_vec);
    }

    return move_vec;
}

void ObserverController::ApplyKeyboardInput(const InputManager& input, Camera& camera)
{
    observer_.prev_position_ = observer_.position_;

    if (!camera.EnabledMovement())
    {
        camera.SetPrevPos(camera.Pos());
    }
    
    const glm::vec3 move_vec = GetMovementVector(input);

    if (glm::length(move_vec) > 0.0f)
    {
        observer_.position_ += move_vec * constants::observer::movement_speed * static_cast<float>(constants::engine::tick_dt);
        observer_.moving_ = true;
        
        if (!camera.EnabledMovement())
        {
            camera.SetPos(observer_.position_);
            //camera.SetPos(observer_.position_ + camera.FPSOffset());
            camera.SetStale(true);
        }
    }
}

void ObserverController::ApplyMovementVector(glm::vec3 move_vec, Camera& camera)
{
    observer_.prev_position_ = observer_.position_;

    if (!camera.EnabledMovement())
    {
        camera.SetPrevPos(camera.Pos());
    }

    if (glm::length(move_vec) > 0.0f)
    {
        observer_.position_ += move_vec * constants::observer::movement_speed * static_cast<float>(constants::engine::tick_dt);
        observer_.moving_ = true;
        
        if (!camera.EnabledMovement())
        {
            camera.SetPos(observer_.position_);
            //camera.SetPos(observer_.position_ + camera.FPSOffset());
            camera.SetStale(true);
        }
    }
}

void ObserverController::ApplyMouseRotation(glm::vec2 mouse_delta, Camera& camera)
{
    observer_.prev_yaw_ = observer_.yaw_;
    observer_.prev_pitch_ = observer_.pitch_;

    if (!camera.EnabledRotation())
    {
        camera.SetPrevYaw(observer_.prev_yaw_);
        camera.SetPrevPitch(observer_.prev_pitch_);
    }

    if (math_utils::FloatingPointNearZero(mouse_delta.x) && math_utils::FloatingPointNearZero(mouse_delta.y))
    {
        return;
    }

    observer_.yaw_ += mouse_delta.x * constants::observer::move_sensitivity;
    observer_.pitch_ = std::clamp(observer_.pitch_ + (mouse_delta.y * constants::observer::move_sensitivity), constants::observer::pitch_min, constants::observer::pitch_max);
    observer_.moving_ = true;

    if (!camera.EnabledRotation())
    {
        camera.SetYaw(observer_.yaw_);
        camera.SetPitch(observer_.pitch_);
        camera.SetStale(true);
    }

    // more TODO later (WoW style camera) 
}