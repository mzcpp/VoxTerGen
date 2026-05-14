#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <SDL2/SDL.h>

#include <glm/vec3.hpp>

#include "utils/Constants.hpp"

enum class MovementState
{
	GROUNDED,
	SWIMMING,
	AIRBORNE,
	NOCLIP
};

class ObserverController;

class Observer
{
	friend class ObserverController;

private:
	MovementState movement_state_;
	
	glm::dvec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	
	float yaw_;
	float pitch_;
	
	glm::dvec3 velocity_;
	
	glm::dvec3 prev_position_;
	float prev_yaw_;
	float prev_pitch_;

	std::size_t width_;
	std::size_t height_;
	std::size_t depth_;
	
	bool noclip_;
	bool moving_;

public:
	Observer(const glm::dvec3& position = constants::geometry::spawn_point, float yaw = 0.0f, float pitch = 0.0f);

	void Tick() noexcept;

	void UpdateObserverVectors();

	void LogObserverData() const;

	// Getters
	MovementState GetMovementState() const noexcept { return movement_state_; }
	glm::dvec3 Pos() const noexcept { return position_; }
	glm::vec3 Front() const noexcept { return front_; }
	glm::vec3 Up() const noexcept { return up_; }
	glm::vec3 Right() const noexcept { return right_; }
	float Yaw() const noexcept { return yaw_; }
	float Pitch() const noexcept { return pitch_; }
	glm::dvec3 PrevPos() const noexcept { return prev_position_; }
    float PrevYaw() const noexcept { return prev_yaw_; }
	float PrevPitch() const noexcept { return prev_pitch_; }
	bool Noclip() const noexcept { return noclip_; }
	std::size_t Width() const noexcept { return width_; }
	std::size_t Height() const noexcept { return height_; }
	std::size_t Depth() const noexcept { return depth_; }
	
	// Setters
	void SetPosition(glm::dvec3 new_pos) noexcept { position_ = new_pos; }
	void SetMovementState(MovementState state) noexcept { movement_state_ = state; }
	void ToggleNoclip() noexcept { noclip_ = !noclip_; }
	void SetWidth(std::size_t width) noexcept { width_ = width; }
	void SetHeight(std::size_t height) noexcept { height_ = height; }
	void SetDepth(std::size_t depth) noexcept { depth_ = depth; }
};

#endif // OBSERVER_HPP

