#include "world/Observer.hpp"

#include <SDL2/SDL.h>

Observer::Observer(const glm::dvec3& position) : 
position_(position), 
prev_position_(position), 
velocity_(glm::dvec3(0.0)), 
movement_state_(MovementState::GROUNDED), 
noclip_(false)
{
}

void Observer::Tick() noexcept
{

}

void Observer::PrintObserverData() const
{
	Logger::Log(LogLevel::DEBUG, "---------------------------- Observer Info ----------------------------");
	Logger::Log(LogLevel::DEBUG, "Position: {} {} {}", position_.x, position_.y, position_.z);
}