#include "input/ObserverController.hpp"
#include "input/InputManager.hpp"
#include "graphics/Camera.hpp"
#include "utils/Constants.hpp"
#include "utils/MathUtils.hpp"

#include <glm/glm.hpp>

#include <SDL2/SDL.h>

#include <algorithm>

void ObserverController::Tick(const InputManager& input, Camera& camera)
{
    ApplyKeyboardInput(input, camera);
    ApplyMouseRotation(input, camera);
}

ObserverController::ObserverController(Observer& observer) : observer_(observer)
{
}

void ObserverController::ApplyKeyboardInput(const InputManager& input, Camera& camera)
{
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
       move_dir += observer_.Up();
    }

    if (input.KeyDown(SDL_SCANCODE_LCTRL))
    {
       move_dir -= observer_.Up();
    }

    if (glm::length(move_dir) > 0.0f)
    {
       move_dir = glm::normalize(move_dir);

       observer_.position_ += move_dir * constants::observer::movement_speed * static_cast<float>(constants::engine::tick_dt);

       observer_.moving_ = true;
    }

    // TODO
    //camera.SetPos(observer_.position_);
}

void ObserverController::ApplyMouseRotation(const InputManager& input, Camera& camera)
{
    observer_.prev_yaw_ = observer_.yaw_;
    observer_.prev_pitch_ = observer_.pitch_;

    const glm::vec2 mouse_delta = input.MouseDelta();

    if (math_utils::FloatingPointNearZero(mouse_delta.x) && math_utils::FloatingPointNearZero(mouse_delta.y))
    {
        return;
    }

    observer_.yaw_ += mouse_delta.x * constants::camera::move_sensitivity;
    observer_.pitch_ = std::clamp(observer_.pitch_ + (mouse_delta.y * constants::camera::move_sensitivity), constants::camera::pitch_min, constants::camera::pitch_max);
    observer_.moving_ = true;

    //camera.SetYaw(observer_.yaw_);
    //camera.SetPitch(observer_.pitch_); 

    // more TODO later (WoW style camera) 
}