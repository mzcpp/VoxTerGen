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

void ObserverController::Tick(glm::vec3 displacement_vector, glm::vec2 mouse_delta, Camera& camera)
{
    ApplyDisplacementVector(displacement_vector, camera);
    ApplyMouseRotation(mouse_delta, camera);
}

glm::vec3 ObserverController::GetDirectionVector(const InputManager& input) const
{
    glm::vec3 dir_vec(0.0f);

    if (input.KeyDown(SDL_SCANCODE_W))
    {
       dir_vec += observer_.Front();
    }

    if (input.KeyDown(SDL_SCANCODE_S))
    {
       dir_vec -= observer_.Front();
    }

    if (input.KeyDown(SDL_SCANCODE_A))
    {
       dir_vec -= observer_.Right();
    }

    if (input.KeyDown(SDL_SCANCODE_D))
    {
       dir_vec += observer_.Right();
    }

    if (input.KeyDown(SDL_SCANCODE_SPACE))
    {
       dir_vec += constants::math::world_up;
    }

    if (input.KeyDown(SDL_SCANCODE_LCTRL))
    {
       dir_vec -= constants::math::world_up;
    }

    if (glm::length(dir_vec) > 0.0f)
    {
       dir_vec = glm::normalize(dir_vec);
    }

    return dir_vec;
}

glm::vec3 ObserverController::GetDisplacementVector(glm::vec3 dir_vec) const
{
    return dir_vec * constants::observer::movement_speed * static_cast<float>(constants::engine::tick_dt);   
}

void ObserverController::ApplyKeyboardInput(const InputManager& input, Camera& camera)
{
    observer_.prev_position_ = observer_.position_;

    if (!camera.EnabledMovement())
    {
        camera.SetPrevPos(camera.Pos());
    }
    
    const glm::vec3 dir_vec = GetDirectionVector(input);

    if (glm::length(dir_vec) > 0.0f)
    {
        observer_.position_ += GetDisplacementVector(dir_vec);
        observer_.moving_ = true;
        
        if (!camera.EnabledMovement())
        {
            camera.SetPos(observer_.position_);
            //camera.SetPos(observer_.position_ + camera.FPSOffset());
            camera.SetStale(true);
        }
    }
}

void ObserverController::ApplyDirectionVector(glm::vec3 dir_vec, Camera& camera)
{
    observer_.prev_position_ = observer_.position_;

    if (!camera.EnabledMovement())
    {
        camera.SetPrevPos(camera.Pos());
    }

    if (glm::length(dir_vec) > 0.0f)
    {
        observer_.position_ += GetDisplacementVector(dir_vec);
        observer_.moving_ = true;
        
        if (!camera.EnabledMovement())
        {
            camera.SetPos(observer_.position_);
            //camera.SetPos(observer_.position_ + camera.FPSOffset());
            camera.SetStale(true);
        }
    }
}

void ObserverController::ApplyDisplacementVector(glm::vec3 displacement_vec, Camera& camera)
{
    observer_.prev_position_ = observer_.position_;

    if (!camera.EnabledMovement())
    {
        camera.SetPrevPos(camera.Pos());
    }

    if (glm::length(displacement_vec) > 0.0f)
    {
        observer_.position_ += displacement_vec;
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