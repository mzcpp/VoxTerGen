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

	void Tick(const InputManager& input, Camera& camera);

	glm::vec3 GetMovementVector(const InputManager& input) const noexcept;

	void ApplyKeyboardInput(const InputManager& input, Camera& camera);

	void ApplyMovementVector(const glm::vec3& move_vec, Camera& camera);

	void ApplyMouseRotation(const InputManager& input, Camera& camera);
};

#endif

