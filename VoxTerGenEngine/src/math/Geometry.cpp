#include "math/Geometry.hpp"
#include "math/Plane.hpp"

#include "physics/AABB.hpp"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <algorithm>
#include <iostream>
#include <cmath>

namespace geometry
{
    bool Intersects(const std::array<Plane, 6>& frustum, const AABB& aabb)
    {
        return std::ranges::all_of(frustum, [&aabb](const Plane& plane) {
            return Intersects(plane, aabb);
        });
    }

    bool Intersects(const Plane& plane, const AABB& aabb)
    {
        const glm::dvec3 furthest{
            plane.normal_.x >= 0.0 ? aabb.max_.x : aabb.min_.x, 
            plane.normal_.y >= 0.0 ? aabb.max_.y : aabb.min_.y, 
            plane.normal_.z >= 0.0 ? aabb.max_.z : aabb.min_.z
        };

        return GetSignedDistance(plane, furthest) >= 0.0;
    }

    bool IsPointInside(const Plane& plane, glm::dvec3 point)
    {
        return GetSignedDistance(plane, point) >= 0.0;
    }

    double GetSignedDistance(const Plane& plane, glm::dvec3 point)
    {
        return glm::dot(plane.normal_, point) + plane.dist_;
    }

    double DistanceSquared(const glm::dvec3 p1, const glm::dvec3 p2)
    {
        const glm::dvec3 delta = p1 - p2;

        return glm::dot(delta, delta);
    }

    double Distance(const glm::dvec3 p1, const glm::dvec3 p2)
    {
        return std::sqrt(DistanceSquared(p1, p2));
    }
} // namespace geometry