#ifndef AABB_HPP
#define AABB_HPP

#include "glm/vec3.hpp"

struct AABB
{
    glm::dvec3 min_;
    glm::dvec3 max_;

    AABB()
    {
        min_ = { 0.0, 0.0, 0.0 };
        max_ = { 0.0, 0.0, 0.0 };
    }

    AABB(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z)
    {
        min_ = { min_x, min_y, min_z };
        max_ = { max_x, max_y, max_z };
    }

    AABB(glm::dvec3 min, glm::dvec3 max)
    {
        min_ = min;
        max_ = max;
    }

    AABB Expand(glm::dvec3 value)
    {
        AABB result;

        if (value.x > 0.0)
        {
            result.max_.x = max_.x + value.x;
        }
        else
        {
            result.min_.x = min_.x + value.x;
        }

        if (value.y > 0.0)
        {
            result.max_.y = max_.y + value.y;
        }
        else
        {
            result.min_.y = min_.y + value.y;
        }

        if (value.z > 0.0)
        {
            result.max_.z = max_.z + value.z;
        }
        else
        {
            result.min_.z = min_.z + value.z;
        }

        return result;
    }

    AABB Grow(glm::dvec3 value)
    {
        AABB result;

        result.max_.x = max_.x + value.x;
        result.max_.y = max_.y + value.y;
        result.max_.z = max_.z + value.z;

        result.min_.x = min_.x - value.x;
        result.min_.y = min_.y - value.y;
        result.min_.z = min_.z - value.z;

        return result;
    }

    void Move(glm::dvec3 vec)
    {
        min_.x += vec.x;
        min_.y += vec.y;
        min_.z += vec.z;
    }
};

#endif