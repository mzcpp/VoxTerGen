#include "world/Observer.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL.h>

#include "utils/Logger.hpp"

Observer::Observer(const glm::dvec3 position, float yaw, float pitch) : 
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
	movement_state_(MovementState::AIRBORNE), 
	width_(constants::observer::width), 
	height_(constants::observer::height), 
	depth_(constants::observer::depth), 
	walk_acceleration_(140.0), 
	walk_friction_(140.0), 
	air_friction_(5.0), 
	max_walk_speed_(10.0), 
	jump_velocity_(7.0), 
	moving_(true)
{
	UpdateObserverVectors();
}	

void Observer::Tick() noexcept
{
    UpdateObserverVectors();
#if _DEBUG
	//LogObserverData();
#endif

	moving_ = false;
}

void Observer::UpdateObserverVectors()
{
	if (!moving_)
	{
		return;
	}

	const glm::vec3 front = { 
		cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)), 
		sin(glm::radians(pitch_)), 
		sin(glm::radians(yaw_)) * cos(glm::radians(pitch_)) 
	};

	front_ = glm::normalize(front);
	right_ = glm::normalize(glm::cross(front_, constants::math::world_up));
	up_ = glm::normalize(glm::cross(right_, front_));
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

void Observer::PrintMovementState() const
{
	switch (movement_state_)
	{
	case MovementState::GROUNDED:
		std::cout << "Grounded" << '\n';
		break;
	case MovementState::AIRBORNE:
		std::cout << "Airborne" << '\n';
		break;
	case MovementState::SWIMMING:
		std::cout << "Swimming" << '\n';
		break;
	}
}
