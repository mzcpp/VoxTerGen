#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cstdint>

struct Vertex
{
    glm::vec3 position_ = glm::vec3(0.0f);
    std::uint8_t normal_ = 0;
    glm::vec2 uv_ = glm::vec2(0.0f);
    std::uint8_t material_ = 0;
};

#endif // VERTEX_HPP
