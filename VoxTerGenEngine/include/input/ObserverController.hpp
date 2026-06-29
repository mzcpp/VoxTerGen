#ifndef OBSERVER_CONTROLLER_HPP
#define OBSERVER_CONTROLLER_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "world/Observer.hpp"
#include "world/Block.hpp"

class Block;
class Camera;
class CollisionSystem;
class InputManager;
class ChunkManager;

class ObserverController
{
private:
	Observer& observer_;
	bool noclip_;

	glm::dvec3 input_direction_;
	glm::dvec3 horizontal_input_direction_;
	glm::vec2 mouse_delta_;

public:
	ObserverController(Observer& observer);

    ObserverController(const ObserverController& other) = delete;
    ObserverController& operator=(const ObserverController& other) = delete;

    ObserverController(ObserverController&& other) = delete;
    ObserverController& operator=(ObserverController&& other) = delete;

	void GatherInput(const InputManager& input_manager);

	void Tick(const CollisionSystem& collision_system, const ChunkManager& chunk_manager, Camera& camera);

	void ApplyChanges(Camera& camera);

	glm::dvec3 GetDirectionVector(const InputManager& input_manager) const;

	glm::dvec3 GetHorizontalDirectionVector(const InputManager& input_manager) const;

	glm::dvec3 GetHorizontalVelocityVector(glm::dvec3 dir_vec) const noexcept;

	glm::dvec3 GetDisplacementVector(glm::dvec3 dir_vec) const noexcept;
	
	glm::dvec3 GetClippedDisplacementVector(glm::dvec3 result_displacement_vector, const CollisionSystem& collision_system, const ChunkManager& chunk_manager) const;

	void ApplyKeyboardInput(const InputManager& input_manager, Camera& camera);

	void ApplyDirectionVector(glm::dvec3 move_vec, Camera& camera);

	void ApplyDisplacementVector(glm::dvec3 displacement_vec, Camera& camera);

	void ApplyMouseRotation(glm::vec2 mouse_delta, Camera& camera);

	BlockInfo GetBlockInfoBelowObserver(const ChunkManager& chunk_manager);

	void ToggleNoclip();

	void DecayObserverVelocity(double friction);

	void ClampObserverVelocity(double max);
};

#endif

