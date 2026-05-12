#ifndef OBSERVER_CONTROLLER_HPP
#define OBSERVER_CONTROLLER_HPP

#include "world/Observer.hpp"

class Camera;
class InputManager;

class ObserverController
{
private:
	Observer& observer_;

public:
	ObserverController(Observer& observer);

    ObserverController(const ObserverController& other) = delete;
    ObserverController& operator=(const ObserverController& other) = delete;

    ObserverController(ObserverController&& other) = delete;
    ObserverController& operator=(ObserverController&& other) = delete;

	void ApplyInput(const InputManager& input, const Camera& camera);

	void ApplyMouseRotation(const InputManager& input, const Camera& camera);
};

#endif

