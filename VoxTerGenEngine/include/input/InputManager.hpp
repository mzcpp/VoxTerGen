#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <glm/vec2.hpp>

#include <SDL2/SDL.h>

#include <unordered_map>
#include <cstdint>

struct MouseState
{
    glm::ivec2 pos_{ 0, 0 };
    glm::vec2 delta_{ 0.0f };
    float wheel_ = 0.0f; 
    std::uint32_t buttons_ = 0;
};

class InputManager
{
private:
    std::unordered_map<SDL_Scancode, bool> keys_;
    std::unordered_map<SDL_Scancode, bool> prev_keys_;
    MouseState mouse_;

public:
    InputManager() = default;

    void ResetFrameState();

    void ProcessEvent(const SDL_Event& e);

    bool KeyPressed(SDL_Scancode key) const;

    bool KeyDown(SDL_Scancode key) const;

    bool KeyReleased(SDL_Scancode key) const;

    bool MouseButtonDown(Uint8 button) const;

    // Getters
    const MouseState& Mouse() const { return mouse_; }
    const glm::vec2& MouseDelta() const { return mouse_.delta_; }
    const glm::ivec2& MousePos() const { return mouse_.pos_; }
    float MouseWheel() const { return mouse_.wheel_; }
};

#endif // INPUT_MANAGER_HPP