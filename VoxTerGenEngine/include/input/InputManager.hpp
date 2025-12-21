#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <glm/vec2.hpp>

#include <SDL2/SDL.h>

#include <unordered_map>

struct MouseState 
{
    glm::ivec2 pos_{ 0, 0 };     // Absolute position
    glm::vec2 delta_{ 0.0f };    // Movement this frame
    float wheel_ = 0.0f;
    Uint32 buttons_ = 0;        // Bitmask of pressed buttons
};

class InputManager
{
private:
    std::unordered_map<SDL_Scancode, bool> keys_;     
    std::unordered_map<SDL_Scancode, bool> prev_keys_;
    MouseState mouse_;

public:
    InputManager() = default;

    // Call at the start of each frame
    void ResetFrameState();

    // Call for every SDL_Event
    void ProcessEvent(const SDL_Event& e);

    // Keyboard queries
    bool KeyPressed(SDL_Scancode key) const; 
    bool KeyDown(SDL_Scancode key) const;    
    bool KeyReleased(SDL_Scancode key) const;

    // Mouse queries
    const MouseState& Mouse() const { return mouse_; }
    bool MouseButtonDown(Uint8 button) const { return mouse_.buttons_ & SDL_BUTTON(button); }
    const glm::vec2& MouseDelta() const { return mouse_.delta_; }
    const glm::ivec2& MousePos() const { return mouse_.pos_; }
    float MouseWheel() const { return mouse_.wheel_; }
};

#endif
