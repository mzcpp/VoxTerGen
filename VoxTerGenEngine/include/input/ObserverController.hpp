#ifndef OBSERVER_CONTROLLER_HPP
#define OBSERVER_CONTROLLER_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "world/Observer.hpp"


class Camera;
class InputManager;
class World;
class CollisionSystem;

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

	void Tick(const InputManager& input_manager, const CollisionSystem& collision_system, const World& world, Camera& camera);

	glm::dvec3 GetDirectionVector(const InputManager& input_manager) const;

	glm::dvec3 GetDisplacementVector(glm::dvec3 dir_vec) const;
	
	glm::dvec3 GetClippedDisplacementVector(const InputManager& input_manager, const CollisionSystem& collision_system, const World& world) const;

	void ApplyKeyboardInput(const InputManager& input_manager, Camera& camera);

	void ApplyDirectionVector(glm::dvec3 move_vec, Camera& camera);

	void ApplyDisplacementVector(glm::dvec3 displacement_vec, Camera& camera);

	void ApplyMouseRotation(glm::vec2 mouse_delta, Camera& camera);
};

#endif

