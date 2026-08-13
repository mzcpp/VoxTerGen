#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include "utils/Logger.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cstdint>
#include <ranges>
#include <cassert>

enum class MajorAxis : std::uint8_t
{
    X = 0,
    Y,
    Z, 
    AxisCount
};

constexpr auto AllAxes()
{
    return std::views::iota(static_cast<std::uint8_t>(0), static_cast<std::uint8_t>(MajorAxis::AxisCount)) |
        std::views::transform([](std::uint8_t i)
            {
                return static_cast<MajorAxis>(i);
            });
}

/**
 * @brief Represents the six cardinal directions for neighboring blocks in a chunk.
 */
enum class Direction : std::uint8_t
{
    PosX = 0,
    NegX,
    PosY,
    NegY,
    PosZ,
    NegZ,
    DirectionCount
};

constexpr auto AllDirections()
{
    return std::views::iota(static_cast<std::uint8_t>(0), static_cast<std::uint8_t>(Direction::DirectionCount)) |
        std::views::transform([](std::uint8_t i)
            {
                return static_cast<Direction>(i);
            });
}

constexpr glm::ivec3 NeighborCoords(const glm::ivec3& coords, Direction dir)
{
    switch (dir)
    {
    case Direction::PosX: return { coords.x + 1, coords.y, coords.z };
    case Direction::NegX: return { coords.x - 1, coords.y, coords.z };
    case Direction::PosY: return { coords.x, coords.y + 1, coords.z };
    case Direction::NegY: return { coords.x, coords.y - 1, coords.z };
    case Direction::PosZ: return { coords.x, coords.y, coords.z + 1 };
    case Direction::NegZ: return { coords.x, coords.y, coords.z - 1 };
    }

    Logger::Log(LogLevel::ERROR, "NeighborCoords received an unknown type of Direction!: dir = {}", static_cast<std::uint8_t>(dir));
    assert(false);
    return coords;
}

constexpr glm::vec3 DirToNormal(Direction dir)
{
    switch (dir)
    {
    case Direction::PosX: return { 1.0f, 0.0f, 0.0f };
    case Direction::NegX: return { -1.0f, 0.0f, 0.0f };
    case Direction::PosY: return { 0.0f, 1.0f, 0.0f };
    case Direction::NegY: return { 0.0f, -1.0f, 0.0f };
    case Direction::PosZ: return { 0.0f, 0.0f, 1.0f };
    case Direction::NegZ: return { 0.0f, 0.0f, -1.0f };
    }

    Logger::Log(LogLevel::ERROR, "DirToNormal received an unknown type of Direction!: dir = {}", static_cast<std::uint8_t>(dir));
    assert(false);
    return { 0.0f, 0.0f, 0.0f };
}

constexpr Direction ToDirection(MajorAxis axis, bool positive) noexcept
{
    const std::uint8_t axis_index = static_cast<std::uint8_t>(axis) * 2;
    
    return static_cast<Direction>(axis_index + (positive ? 0 : 1));
}

#endif // DIRECTION_HPP
