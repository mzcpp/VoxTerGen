#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <SDL2/SDL.h>

#include <glm/vec3.hpp>

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
	glm::dvec3 prev_position_;
	glm::dvec3 velocity_;
	//float yaw_;
	//float prev_yaw_;
	//float pitch_;
	//float prev_pitch_;
	bool noclip_;

public:
	Observer(const glm::dvec3& position = glm::dvec3{ 0.0, 1.0, 0.0 });

	void Tick() noexcept;

	void PrintObserverData() const;

	// Getters
	bool Noclip() const noexcept { return noclip_; }
	
	// Setters
	void ToggleNoclip() noexcept { noclip_ = !noclip_; }
};

#endif // OBSERVER_HPP

