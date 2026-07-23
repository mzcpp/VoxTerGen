#ifndef MATH_HPP
#define MATH_HPP

#include <glm/vec3.hpp>

#include <array>

struct AABB;
struct Plane;

namespace geometry
{
    bool Intersects(const std::array<Plane, 6>& frustum, const AABB& aabb);

    bool Intersects(const Plane& plane, const AABB& aabb);

    bool PlaneContainsPoint(const Plane& plane, glm::vec3 point);
} // namespace geometry

#endif