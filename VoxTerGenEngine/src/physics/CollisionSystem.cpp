#include "physics/AABB.hpp"
#include "physics/CollisionSystem.hpp"

#include <iostream>

/**
 * @note Portion of this source code was adapted from an article by Andre Blunt.
 * Source: https://medium.com/@andrebluntindie/3d-aabb-collision-detection-and-resolution-for-voxel-games-5fcbfdb8cdb4
 */

CollisionSystem::CollisionSystem()
{
}

bool CollisionSystem::AABBIntersectsX(AABB first, AABB second) const noexcept
{
    if (first.min_.x < second.max_.x && first.max_.x > second.min_.x)
    {
        //std::cout << "x\n";
    }

    return first.min_.x < second.max_.x && first.max_.x > second.min_.x; 
}

bool CollisionSystem::AABBIntersectsY(AABB first, AABB second) const noexcept
{
    if (first.min_.y < second.max_.y && first.max_.y > second.min_.y)
    {
        //std::cout << "y\n";
    }

    return first.min_.y < second.max_.y && first.max_.y > second.min_.y;
}

bool CollisionSystem::AABBIntersectsZ(AABB first, AABB second) const noexcept
{
    if (first.min_.z < second.max_.z && first.max_.z > second.min_.z)
    {
        //std::cout << "z\n";
    }

    return first.min_.z < second.max_.z && first.max_.z > second.min_.z;
}

bool CollisionSystem::AABBIntersects(AABB first, AABB second) const noexcept
{
    return AABBIntersectsX(first, second) && AABBIntersectsY(first, second) && AABBIntersectsZ(first, second);
}

float CollisionSystem::GetClipX(AABB first, AABB second, float delta_x) const noexcept
{
    if (!AABBIntersectsY(first, second) || !AABBIntersectsZ(first, second))
    {
        return delta_x;
    }

    if (delta_x > 0 && first.max_[0] <= second.min_[0])
    {
        const float clip = second.min_[0] - first.max_[0];

        if (clip < delta_x)
        {
            delta_x = clip;
        }
    }

    if (delta_x < 0 && first.min_[0] >= second.max_[0])
    {
        const float clip = second.max_[0] - first.min_[0];

        if (clip > delta_x)
        {
            delta_x = clip;
        }
    }

    return delta_x;
}

float CollisionSystem::GetClipY(AABB first, AABB second, float delta_y) const noexcept
{
    if (!AABBIntersectsX(first, second) || !AABBIntersectsZ(first, second))
    {
        return delta_y;
    }

    if (delta_y > 0 && first.max_[1] <= second.min_[1])
    {
        const float clip = second.min_[1] - first.max_[1];

        if (clip < delta_y)
        {
            delta_y = clip;
        }
    }

    if (delta_y < 0 && first.min_[1] >= second.max_[1])
    {
        const float clip = second.max_[1] - first.min_[1];

        if (clip > delta_y)
        {
            delta_y = clip;
        }
    }

    return delta_y;
}

float CollisionSystem::GetClipZ(AABB first, AABB second, float delta_z) const noexcept
{
    if (!AABBIntersectsX(first, second) || !AABBIntersectsY(first, second))
    {
        return delta_z;
    }

    if (delta_z > 0 && first.max_[2] <= second.min_[2])
    {
        const float clip = second.min_[2] - first.max_[2];

        if (clip < delta_z)
        {
            delta_z = clip;
        }
    }

    if (delta_z < 0 && first.min_[2] >= second.max_[2])
    {
        const float clip = second.max_[2] - first.min_[2];

        if (clip > delta_z)
        {
            delta_z = clip;
        }
    }

    return delta_z;
}