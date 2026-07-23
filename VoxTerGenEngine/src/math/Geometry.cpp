#include "math/Geometry.hpp"
#include "math/Plane.hpp"

#include "physics/AABB.hpp"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <ranges>

namespace geometry
{
    bool Geometry::Intersects(const std::array<Plane, 6>& frustum, const AABB& aabb)
    {
        return std::ranges::any_of(frustum, [&aabb](const Plane& plane) {
            return Intersects(plane, aabb);
        });
    }

    bool Geometry::Intersects(const Plane& plane, const AABB& aabb)
    {
        return std::ranges::any_of(aabb.Corners(), [&plane](glm::dvec3 point) {
            return PlaneContainsPoint(plane, point);
        });
    }

    bool Geometry::PlaneContainsPoint(const Plane& plane, glm::vec3 point)
    {
        return true;
    }
} // namespace geometry