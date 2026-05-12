#include "input/ObserverController.hpp"
#include "input/InputManager.hpp"
#include "graphics/Camera.hpp"

#include <glm/glm.hpp>

#include <SDL2/SDL.h>

ObserverController::ObserverController(Observer& observer) : observer_(observer)
{
}

void ObserverController::ApplyKeyboardInput(const InputManager& input, const Camera& camera)
{
    //if (!input.StateChanged())
    //{
    //    return;
    //}

    //glm::vec3 move_dir(0.0f);

    //if (input.KeyDown(SDL_SCANCODE_W))
    //{
    //    move_dir += camera.Front();
    //}

    //if (input.KeyDown(SDL_SCANCODE_S))
    //{
    //    move_dir -= camera.Front();
    //}

    //if (input.KeyDown(SDL_SCANCODE_A))
    //{
    //    move_dir -= camera.Right();
    //}

    //if (input.KeyDown(SDL_SCANCODE_D))
    //{
    //    move_dir += camera.Right();
    //}

    //if (input.KeyDown(SDL_SCANCODE_SPACE))
    //{
    //    move_dir += camera.Up();
    //}

    //if (input.KeyDown(SDL_SCANCODE_LCTRL))
    //{
    //    move_dir -= camera.Up();
    //}

    //if (glm::length(move_dir) > 0.0f)
    //{
    //    move_dir = glm::normalize(move_dir);

    //    observer_.position_ += move_dir * movement_speed_ * delta_time;

    //    observer_.changed_ = true;
    //    observer_.moving_ = true;
    //}
}

void ObserverController::ApplyMouseRotation(const InputManager& input, const Camera& camera)
{
    // TODO: 
}