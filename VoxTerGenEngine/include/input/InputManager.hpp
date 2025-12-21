#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <glm/vec2.hpp>

#include <SDL2/SDL.h>

#include <unordered_map>

/**
 * @brief Represents the state of the mouse for a single frame.
 */
struct MouseState
{
    glm::ivec2 pos_{ 0, 0 };
    glm::vec2 delta_{ 0.0f };
    float wheel_ = 0.0f; 
    Uint32 buttons_ = 0;
};

/**
 * @brief Handles keyboard and mouse input state.
 *
 * InputManager tracks keyboard and mouse states frame-to-frame,
 * allowing queries for key presses, releases, and mouse movement.
 */
class InputManager
{
private:
    std::unordered_map<SDL_Scancode, bool> keys_;
    std::unordered_map<SDL_Scancode, bool> prev_keys_;
    MouseState mouse_;

public:
    InputManager() = default;

    /**
     * @brief Resets per-frame input state.
     *
     * Call at the start of each frame to reset mouse delta, wheel,
     * and store previous key states.
     */
    void ResetFrameState();

    /**
     * @brief Processes a single SDL event to update input state.
     *
     * @param e SDL_Event received from the event queue
     */
    void ProcessEvent(const SDL_Event& e);

    // Keyboard queries
    /**
     * @brief Checks if a key was pressed this frame (went from up to down).
     *
     * @param key SDL scancode of the key
     * 
     * @return True if the key was just pressed
     */
    bool KeyPressed(SDL_Scancode key) const;

    /**
     * @brief Checks if a key is currently held down.
     *
     * @param key SDL scancode of the key
     * 
     * @return True if the key is down
     */
    bool KeyDown(SDL_Scancode key) const;

    /**
     * @brief Checks if a key was released this frame (went from down to up).
     *
     * @param key SDL scancode of the key
     * 
     * @return True if the key was just released
     */
    bool KeyReleased(SDL_Scancode key) const;

    // Getters
    /**
     * @brief Returns the current mouse state.
     * 
     * @return Reference to the MouseState
     */
    const MouseState& Mouse() const { return mouse_; }

    /**
     * @brief Checks if a mouse button is currently pressed.
     * 
     * @param button SDL mouse button index
     * 
     * @return True if the button is pressed
     */
    bool MouseButtonDown(Uint8 button) const { return mouse_.buttons_ & SDL_BUTTON(button); }

    /**
     * @brief Returns the mouse movement delta since the last frame.
     * 
     * @return Mouse movement as glm::vec2
     */
    const glm::vec2& MouseDelta() const { return mouse_.delta_; }

    /**
     * @brief Returns the current mouse position.
     * 
     * @return Mouse position as glm::ivec2
     */
    const glm::ivec2& MousePos() const { return mouse_.pos_; }

    /**
     * @brief Returns the mouse wheel delta for the current frame.
     * 
     * @return Wheel delta as float
     */
    float MouseWheel() const { return mouse_.wheel_; }
};

#endif // INPUT_MANAGER_HPP