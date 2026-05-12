#include "world/Observer.hpp"

#include <SDL2/SDL.h>

#include "utils/Logger.hpp"

Observer::Observer(const glm::dvec3& position, float yaw, float pitch) : 
	position_(position), 
	front_(glm::vec3(0.0f, 0.0f, -1.0f)), 
	up_(glm::vec3(0.0f)), 
	right_(glm::vec3(0.0f)), 
	yaw_(yaw), 
	pitch_(pitch), 
	prev_position_(position), 
    prev_yaw_(yaw), 
	prev_pitch_(pitch), 
	velocity_(glm::dvec3(0.0)), 
	movement_state_(MovementState::GROUNDED), 
	noclip_(false)
{
}

void Observer::Tick() noexcept
{

}

void Observer::LogObserverData() const
{
	Logger::Log(LogLevel::DEBUG, "---------------------------- Observer Info ----------------------------");
	Logger::Log(LogLevel::DEBUG, "Position: {} {} {}", position_.x, position_.y, position_.z);
	Logger::Log(LogLevel::DEBUG, "Front: {} {} {}", front_.x, front_.y, front_.z);
	Logger::Log(LogLevel::DEBUG, "Up: {} {} {}", up_.x, up_.y, up_.z);
	Logger::Log(LogLevel::DEBUG, "Right: {} {} {}", right_.x, right_.y, right_.z);
	Logger::Log(LogLevel::DEBUG, "Yaw: {}", yaw_);
	Logger::Log(LogLevel::DEBUG, "Pitch: {}", pitch_);
}