#include "input/InputManager.hpp"

#include <SDL2/SDL.h>

#include <cstdint>

void InputManager::ResetFrameState()
{
    prev_keys_ = keys_;
    mouse_.delta_ = glm::vec2(0.0f);
    mouse_.wheel_ = 0.0f;
}

void InputManager::ProcessEvent(const SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_KEYDOWN:
        keys_[e.key.keysym.scancode] = true;
        break;

    case SDL_KEYUP:
        keys_[e.key.keysym.scancode] = false;
        break;

    case SDL_MOUSEMOTION:
        mouse_.delta_.x += static_cast<float>(e.motion.xrel);
        mouse_.delta_.y += static_cast<float>(-e.motion.yrel);
        mouse_.pos_.x = e.motion.x;
        mouse_.pos_.y = e.motion.y;
        break;
    case SDL_MOUSEWHEEL:
        mouse_.wheel_ += e.wheel.preciseY;
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        mouse_.buttons_ = SDL_GetMouseState(&mouse_.pos_.x, &mouse_.pos_.y);
        break;
    default:
        break;
    }
}

bool InputManager::KeyPressed(SDL_Scancode key) const
{
    const bool curr = keys_.contains(key) ? keys_.at(key) : false;
    const bool prev = prev_keys_.contains(key) ? prev_keys_.at(key) : false;
    return curr && !prev;
}

bool InputManager::KeyDown(SDL_Scancode key) const
{
    return keys_.contains(key) ? keys_.at(key) : false;
}

bool InputManager::KeyReleased(SDL_Scancode key) const
{
    const bool curr = keys_.contains(key) ? keys_.at(key) : false;
    const bool prev = prev_keys_.contains(key) ? prev_keys_.at(key) : false;
    return !curr && prev;
}

bool InputManager::MouseButtonDown(std::uint8_t button) const 
{
    return mouse_.buttons_ & SDL_BUTTON(button); 
}