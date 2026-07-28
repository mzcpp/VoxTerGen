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

constexpr glm::ivec3 NeighborCoords(const glm::ivec3& block_coords, Direction dir)
{
    switch (dir)
    {
    case Direction::PosX: return { block_coords.x + 1, block_coords.y, block_coords.z };
    case Direction::NegX: return { block_coords.x - 1, block_coords.y, block_coords.z };
    case Direction::PosY: return { block_coords.x, block_coords.y + 1, block_coords.z };
    case Direction::NegY: return { block_coords.x, block_coords.y - 1, block_coords.z };
    case Direction::PosZ: return { block_coords.x, block_coords.y, block_coords.z + 1 };
    case Direction::NegZ: return { block_coords.x, block_coords.y, block_coords.z - 1 };
    }

    Logger::Log(LogLevel::ERROR, "NeighborCoords received an unknown type of Direction!: dir = {}", static_cast<std::uint8_t>(dir));
    assert(false);
    return block_coords;
}

constexpr std::uint8_t DirToNormal(Direction dir)
{
    switch (dir)
    {
    case Direction::PosX: return 0;
    case Direction::NegX: return 1;
    case Direction::PosY: return 2;
    case Direction::NegY: return 3;
    case Direction::PosZ: return 4;
    case Direction::NegZ: return 5;
    }

    Logger::Log(LogLevel::ERROR, "DirToNormal received an unknown type of Direction!: dir = {}", static_cast<std::uint8_t>(dir));
    assert(false);
    return 0;
}

constexpr Direction ToDirection(MajorAxis axis, bool positive) noexcept
{
    const std::uint8_t axis_index = static_cast<std::uint8_t>(axis) * 2;
    
    return static_cast<Direction>(axis_index + (positive ? 0 : 1));
}

#endif // DIRECTION_HPP
