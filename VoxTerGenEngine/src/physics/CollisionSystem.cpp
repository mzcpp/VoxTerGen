#include "physics/CollisionSystem.hpp"

/**
 * @note Portion of this source code was adapted from an article by Andre Blunt.
 * Source: https://medium.com/@andrebluntindie/3d-aabb-collision-detection-and-resolution-for-voxel-games-5fcbfdb8cdb4
 */

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

float CollisionSystem::GetClipX(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b, float delta_x) const noexcept
{
    if (!AABBIntersectsY(min_a, max_a, min_b, max_b) || !AABBIntersectsZ(min_a, max_a, min_b, max_b))
    {
        return delta_x;
    }

    if (delta_x > 0 && max_a[0] <= min_b[0])
    {
        if (const float clip = min_b[0] - max_a[0] < delta_x)
        {
            delta_x = clip;
        }
    }

    if (delta_x < 0 && min_a[0] >= max_b[0])
    {
        if (const float clip = max_b[0] - min_a[0] > delta_x)
        {
            delta_x = clip;
        }
    }

    return delta_x;
}

float CollisionSystem::GetClipY(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b, float delta_y) const noexcept
{
    if (!AABBIntersectsX(min_a, max_a, min_b, max_b) || !AABBIntersectsZ(min_a, max_a, min_b, max_b))
    {
        return delta_y;
    }

    if (delta_y > 0 && max_a[1] <= min_b[1])
    {
        if (const float clip = min_b[1] - max_a[1] < delta_y)
        {
            delta_y = clip;
        }
    }

    if (delta_y < 0 && min_a[1] >= max_b[1])
    {
        if (const float clip = max_b[1] - min_a[1] > delta_y)
        {
            delta_y = clip;
        }
    }

    return delta_y;
}

float CollisionSystem::GetClipZ(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b, float delta_z) const noexcept
{
    if (!AABBIntersectsX(min_a, max_a, min_b, max_b) || !AABBIntersectsY(min_a, max_a, min_b, max_b))
    {
        return delta_z;
    }

    if (delta_z > 0 && max_a[2] <= min_b[2])
    {
        if (const float clip = min_b[2] - max_a[2] < delta_z)
        {
            delta_z = clip;
        }
    }

    if (delta_z < 0 && min_a[2] >= max_b[2])
    {
        if (const float clip = max_b[2] - min_a[2] > delta_z)
        {
            delta_z = clip;
        }
    }

    return delta_z;
}