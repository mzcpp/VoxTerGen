#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "glm/vec3.hpp"

#include <concepts>

class Block;

template <typename Fnc>
concept BlockQuery = std::invocable<Fnc, glm::ivec3> && std::convertible_to<std::invoke_result_t<Fnc, glm::ivec3>, Block>;

class CollisionSystem
{
private:

public:
    CollisionSystem();

    bool AABBIntersectsX(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept;

    bool AABBIntersectsY(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept;

    bool AABBIntersectsZ(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept;

    bool AABBIntersects(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b) const noexcept;

    float GetClipX(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b, float delta_x) const noexcept;

    float GetClipY(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b, float delta_y) const noexcept;

    float GetClipZ(glm::dvec3 min_a, glm::dvec3 max_a, glm::dvec3 min_b, glm::dvec3 max_b, float delta_z) const noexcept;

    glm::vec3 GetClippedMovementVector(BlockQuery auto&& world_block_query, const Observer& observer, glm::vec3 movement_vector) const noexcept
    {
        glm::vec3 clipped_movement_vector = movement_vector;
        const glm::dvec3 observer_min_coords = observer.Pos();
        const glm::dvec3 observer_max_coords = { observer_min_coords.x + observer.GetWidth(), observer_min_coords.y + observer.GetHeight(), observer_min_coords.z + observer.GetDepth() };
        const glm::ivec3 observer_block_coords = observer.BlockPos();
        const glm::ivec3 start_coords = { observer_block_coords.x - 1, observer_block_coords.y - 1, observer_block_coords.z - 1 };

        std::vector<std::dvec3> neighbor_block_coords;

        for (int y = 0; y < static_cast<int>(observer.GetHeight()) + 2; ++y)
        {
            for (int z = 0; z < static_cast<int>(observer.GetDepth()) + 2; ++z)
            {
                for (int x = 0; x < static_cast<int>(observer.GetWidth()) + 2; ++x)
                {
                    const glm::ivec3 neighbor_block_coords = { start_coords.x + x, start_coords.y + y, start_coords.z + z };
                    const Block neighbor_block = world_block_query(neighbor_block_coords);

                    if (!neighbor_block.IsSolid())
                    {
                        continue;
                    }

                    neighbor_block_coords.push_back({ static_cast<double>(neighbor_block_coords.x), static_cast<double>(neighbor_block_coords.y), static_cast<double>(neighbor_block_coords.z) });
                }
            }
        }

        for (std::dvec3 neighbor_min_pos : neighbor_block_coords)
        {
            // TODO
            const std::dvec3 neighbor_max_pos = { neighbor_min_pos.x + 1.0, neighbor_min_pos.y + 1.0, neighbor_min_pos.z + 1.0 };
            clipped_movement_vector.x = GetClipX(observer_min_coords, observer_max_coords, neighbor_min_pos, neighbor_max_pos, movement_vector.x);
            clipped_movement_vector.y = GetClipY(observer_min_coords, observer_max_coords, neighbor_min_pos, neighbor_max_pos, movement_vector.y);
            clipped_movement_vector.z = GetClipZ(observer_min_coords, observer_max_coords, neighbor_min_pos, neighbor_max_pos, movement_vector.z);
        }

        return clipped_movement_vector;
    }
};

#endif // COLLISION_SYSTEM_HPP

