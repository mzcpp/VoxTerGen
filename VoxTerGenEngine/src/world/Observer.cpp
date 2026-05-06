#include "world/Observer.hpp"

#include <SDL2/SDL.h>

Observer::Observer(const glm::dvec3& position) : pos_(position), movement_state_(MovementState::GROUNDED), noclip_(false)
{

}

void Observer::HandleEvents(SDL_Event e) noexcept
{

}

void Observer::Tick() noexcept
{

}