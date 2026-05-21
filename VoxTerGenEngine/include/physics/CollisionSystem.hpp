#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "glm/vec3.hpp"

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
};

#endif // COLLISION_SYSTEM_HPP

