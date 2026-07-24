#ifndef PLANE_HPP
#define PLANE_HPP

#include <glm/vec3.hpp>

/**
 * @brief Represents a plane in 3D space.
 *
 * A Plane is defined by a normal vector and a distance from the origin.
 * It is primarily used for view-frustum representation and culling.
 */
struct Plane
{
    glm::dvec3 normal_ = { 0.0, 0.0, 0.0 };
	double dist_ = 0.0;
};

#endif