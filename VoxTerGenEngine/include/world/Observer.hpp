#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <SDL2/SDL.h>

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
	MovementState movement_state_;
	bool noclip_;

public:
	Observer();

	void HandleEvents(SDL_Event e) noexcept;

	void Tick() noexcept;

	// Getters
	bool Noclip() const noexcept { return noclip_; }
	
	// Setters
	void SetNoclip(bool noclip) noexcept { noclip_ = noclip; }
};

#endif // OBSERVER_HPP

