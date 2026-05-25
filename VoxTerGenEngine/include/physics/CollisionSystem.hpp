#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "glm/vec3.hpp"

#include <concepts>

class Block;

template <typename Fnc>
concept BlockQuery = std::invocable<Fnc, glm::ivec3> && std::convertible_to<std::invoke_result_t<Fnc, glm::ivec3>, Block>;

class CollisionSystem
{
private:

public:
    CollisionSystem();

    bool AABBIntersectsX(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept;

    bool AABBIntersectsY(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept;

    bool AABBIntersectsZ(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept;

    bool AABBIntersects(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept;

    float GetClipX(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b, float delta_x) const noexcept;

    float GetClipY(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b, float delta_y) const noexcept;

    float GetClipZ(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b, float delta_z) const noexcept;

    glm::vec3 GetClippedMovementVector(BlockQuery auto&& world_block_query, glm::dvec3 player_pos, glm::vec3 movement_vector) const noexcept
    {
        glm::vec3 clipped_movement_vector = movement_vector;

        // get the blocks around player pos
        // 
        // in loop, for each block 

            /*clipped_movement_vector.x = GetClipX(movement_vector.x);
            clipped_movement_vector.y = GetClipY(movement_vector.y);
            clipped_movement_vector.z = GetClipZ(movement_vector.z);*/

        return clipped_movement_vector;
    }
};

#endif // COLLISION_SYSTEM_HPP

