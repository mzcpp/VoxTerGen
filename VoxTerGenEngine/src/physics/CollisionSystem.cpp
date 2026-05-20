#include "physics/CollisionSystem.hpp"

CollisionSystem::CollisionSystem()
{
}


bool CollisionSystem::AABBIntersectsX(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept
{
    return min_a.x < max_b.x && max_a.x > min_b.x; 
}

bool CollisionSystem::AABBIntersectsY(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept
{
    return min_a.y < max_b.y && max_a.y > min_b.y;
}

bool CollisionSystem::AABBIntersectsZ(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept
{
    return min_a.z < max_b.z && max_a.z > min_b.z;
}

bool CollisionSystem::AABBIntersects(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept
{
    return AABBIntersectsX(min_a, max_a, min_b, max_b) && AABBIntersectsY(min_a, max_a, min_b, max_b) && AABBIntersectsZ(min_a, max_a, min_b, max_b);
}