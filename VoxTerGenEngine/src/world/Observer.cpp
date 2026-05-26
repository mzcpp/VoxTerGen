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
	movement_state_(MovementState::GROUNDED), 
	width_(constants::observer::width), 
	height_(constants::observer::height), 
	depth_(constants::observer::depth), 
	noclip_(false), 
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

glm::ivec3 Observer::BlockPos() const noexcept
{
	glm::ivec3 block_pos(0);

	if (position_.x < 0.0)
	{
		block_pos.x = static_cast<int>(std::floor(position_.x));
	}
	else
	{
		block_pos.x = static_cast<int>(position_.x);
	}

	if (position_.y < 0.0)
	{
		block_pos.y = static_cast<int>(std::floor(position_.y));
	}
	else
	{
		block_pos.y = static_cast<int>(position_.y);
	}

	if (position_.z < 0.0)
	{
		block_pos.z = static_cast<int>(std::floor(position_.z));
	}
	else
	{
		block_pos.z = static_cast<int>(position_.z);
	}

	return block_pos;
}