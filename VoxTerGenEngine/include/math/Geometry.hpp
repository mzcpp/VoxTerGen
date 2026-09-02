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

    bool IsPointInside(const Plane& plane, glm::dvec3 point);

    double GetSignedDistance(const Plane& plane, glm::dvec3 point);

    double DistanceSquared(const glm::dvec3 p1, const glm::dvec3 p2);
    
    double Distance(const glm::dvec3 p1, const glm::dvec3 p2);

} // namespace geometry

#endif