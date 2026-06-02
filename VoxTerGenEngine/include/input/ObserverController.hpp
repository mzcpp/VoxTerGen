#ifndef OBSERVER_CONTROLLER_HPP
#define OBSERVER_CONTROLLER_HPP

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

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

	void Tick(glm::vec3 displacement_vector, glm::vec2 mouse_delta, Camera& camera);

	glm::vec3 GetDirectionVector(const InputManager& input) const;

	glm::vec3 GetDisplacementVector(glm::vec3 dir_vec) const;

	void ApplyKeyboardInput(const InputManager& input, Camera& camera);

	void ApplyDirectionVector(glm::vec3 move_vec, Camera& camera);

	void ApplyDisplacementVector(glm::vec3 displacement_vec, Camera& camera);

	void ApplyMouseRotation(glm::vec2 mouse_delta, Camera& camera);
};

#endif

