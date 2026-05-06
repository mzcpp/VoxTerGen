#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <SDL2/SDL.h>

#include <glm/vec3.hpp>

enum class MovementState
{
	GROUNDED,
	SWIMMING,
	FLYING,
	AIRBORNE
};

class Observer
{
private:
	glm::dvec3 pos_;
	glm::dvec3 prev_pos_;
	glm::dvec3 velocity_;
	MovementState movement_state_;
	bool noclip_;

public:
	Observer(const glm::dvec3& position);

	void HandleEvents(SDL_Event e) noexcept;

	void Tick() noexcept;

	// Getters
	bool Noclip() const noexcept { return noclip_; }
	
	// Setters
	void SetNoclip(bool noclip) noexcept { noclip_ = noclip; }
};

#endif // OBSERVER_HPP

