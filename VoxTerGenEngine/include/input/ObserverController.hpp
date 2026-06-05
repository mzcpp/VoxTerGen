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

	void Tick(glm::dvec3 displacement_vector, glm::vec2 mouse_delta, Camera& camera);

	glm::dvec3 GetDirectionVector(const InputManager& input) const;

	glm::dvec3 GetDisplacementVector(glm::dvec3 dir_vec) const;

	void ApplyKeyboardInput(const InputManager& input, Camera& camera);

	void ApplyDirectionVector(glm::dvec3 move_vec, Camera& camera);

	void ApplyDisplacementVector(glm::dvec3 displacement_vec, Camera& camera);

	void ApplyMouseRotation(glm::vec2 mouse_delta, Camera& camera);
};

#endif

