#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <glm/vec2.hpp>

#include <SDL2/SDL.h>

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

struct MouseState
{
    glm::ivec2 pos_ = { 0, 0 };
    glm::vec2 delta_ = { 0.0f, 0.0f };
    float wheel_ = 0.0f; 
    std::uint32_t buttons_ = 0;
};

class InputManager
{
private:
    std::unordered_map<SDL_Scancode, bool> keys_;
    std::unordered_set<SDL_Scancode> pressed_;
    std::unordered_set<SDL_Scancode> released_;
    MouseState mouse_;

public:
    InputManager() = default;

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    void ProcessEvent(const SDL_Event& e);

    bool KeyDown(SDL_Scancode key) const;
    
    bool KeyPressed(SDL_Scancode key) const;

    bool KeyReleased(SDL_Scancode key) const;

    bool MouseButtonDown(std::uint8_t button) const;
    
    void ResetFrameState();

    // Getters
    const MouseState& Mouse() const noexcept { return mouse_; }
    glm::vec2 MouseDelta() const noexcept { return mouse_.delta_; }
    glm::ivec2 MousePos() const noexcept { return mouse_.pos_; }
    float MouseWheel() const noexcept { return mouse_.wheel_; }
};

#endif // INPUT_MANAGER_HPP