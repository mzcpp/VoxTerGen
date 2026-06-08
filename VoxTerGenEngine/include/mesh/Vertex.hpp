#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cstdint>

// TODO: pack into a single integer

struct Vertex
{
    glm::vec3 position_;
    glm::vec3 normal_;
    glm::vec2 uv_;
    std::uint8_t material_;
};

#endif // VERTEX_HPP
