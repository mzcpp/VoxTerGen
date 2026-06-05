#include "physics/AABB.hpp"
#include "physics/CollisionSystem.hpp"

#include "utils/Constants.hpp"

/**
 * @note Portion of this source code was adapted from an article by Andre Blunt.
 * Source: https://medium.com/@andrebluntindie/3d-aabb-collision-detection-and-resolution-for-voxel-games-5fcbfdb8cdb4
 */

CollisionSystem::CollisionSystem()
{
}

bool CollisionSystem::AABBIntersectsX(AABB first, AABB second) const noexcept
{
    return first.min_.x < second.max_.x && first.max_.x > second.min_.x; 
}

bool CollisionSystem::AABBIntersectsY(AABB first, AABB second) const noexcept
{
    return first.min_.y < second.max_.y && first.max_.y > second.min_.y;
}

bool CollisionSystem::AABBIntersectsZ(AABB first, AABB second) const noexcept
{
    return first.min_.z < second.max_.z && first.max_.z > second.min_.z;
}

bool CollisionSystem::AABBIntersects(AABB first, AABB second) const noexcept
{
    return AABBIntersectsX(first, second) && AABBIntersectsY(first, second) && AABBIntersectsZ(first, second);
}

double CollisionSystem::GetClipX(AABB first, AABB second, double delta_x) const noexcept
{
    if (!AABBIntersectsY(first, second) || !AABBIntersectsZ(first, second))
    {
        return delta_x;
    }

    if (delta_x > 0 && first.max_.x <= second.min_.x)
    {
        const double clip = second.min_.x - first.max_.x - constants::physics::collision_epsilon;

        if (clip < delta_x)
        {
            delta_x = clip;
        }
    }

    if (delta_x < 0 && first.min_.x >= second.max_.x)
    {
        const double clip = second.max_.x - first.min_.x + constants::physics::collision_epsilon;

        if (clip > delta_x)
        {
            delta_x = clip;
        }
    }

    return delta_x;
}

double CollisionSystem::GetClipY(AABB first, AABB second, double delta_y) const noexcept
{
    if (!AABBIntersectsX(first, second) || !AABBIntersectsZ(first, second))
    {
        return delta_y;
    }

    if (delta_y > 0 && first.max_.y <= second.min_.y)
    {
        const double clip = second.min_.y - first.max_.y - constants::physics::collision_epsilon;

        if (clip < delta_y)
        {
            delta_y = clip;
        }
    }

    if (delta_y < 0 && first.min_.y >= second.max_.y)
    {
        const double clip = second.max_.y - first.min_.y + constants::physics::collision_epsilon;

        if (clip > delta_y)
        {
            delta_y = clip;
        }
    }

    return delta_y;
}

double CollisionSystem::GetClipZ(AABB first, AABB second, double delta_z) const noexcept
{
    if (!AABBIntersectsX(first, second) || !AABBIntersectsY(first, second))
    {
        return delta_z;
    }

    if (delta_z > 0 && first.max_.z <= second.min_.z)
    {
        const double clip = second.min_.z - first.max_.z - constants::physics::collision_epsilon;

        if (clip < delta_z)
        {
            delta_z = clip;
        }
    }

    if (delta_z < 0 && first.min_.z >= second.max_.z)
    {
        const double clip = second.max_.z - first.min_.z + constants::physics::collision_epsilon;

        if (clip > delta_z)
        {
            delta_z = clip;
        }
    }

    return delta_z;
}