#include "input/InputManager.hpp"

#include <SDL2/SDL.h>

#include <cstdint>

void InputManager::ProcessEvent(const SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_KEYDOWN:
    {
        const SDL_Scancode scancode = e.key.keysym.scancode;

        if (!e.key.repeat && !KeyDown(scancode))
        {
            keys_[scancode] = true;
            pressed_.insert(scancode);
        }
        break;
    }
    case SDL_KEYUP:
    {
        const SDL_Scancode scancode = e.key.keysym.scancode;

        if (KeyDown(scancode))
        {
            keys_[scancode] = false;
            released_.insert(scancode);
        }
        break;
    }
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

bool InputManager::KeyDown(SDL_Scancode key) const
{
    return keys_.contains(key) ? keys_.at(key) : false;
}

bool InputManager::KeyPressed(SDL_Scancode key) const
{
    return pressed_.contains(key);
}

bool InputManager::KeyReleased(SDL_Scancode key) const
{
    return released_.contains(key);
}

bool InputManager::MouseButtonDown(std::uint8_t button) const 
{
    return mouse_.buttons_ & SDL_BUTTON(button); 
}

void InputManager::ResetFrameState()
{
    pressed_.clear();
    released_.clear();
    mouse_.delta_ = {};
    mouse_.wheel_ = 0.0f;
}