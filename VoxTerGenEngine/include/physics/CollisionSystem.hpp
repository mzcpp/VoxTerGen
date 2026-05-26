#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "physics/AABB.hpp"
#include "world/Observer.hpp"
#include "world/Block.hpp"

#include "glm/vec3.hpp"

#include <concepts>
#include <vector>
#include <cmath>

class Block;
class Observer;

template <typename Fnc>
concept BlockQuery = std::invocable<Fnc, glm::ivec3> && std::convertible_to<std::invoke_result_t<Fnc, glm::ivec3>, Block>;

class CollisionSystem
{
private:

public:
    CollisionSystem();

    bool AABBIntersectsX(AABB first, AABB second) const noexcept;

    bool AABBIntersectsY(AABB first, AABB second) const noexcept;

    bool AABBIntersectsZ(AABB first, AABB second) const noexcept;

    bool AABBIntersects(AABB first, AABB second) const noexcept;

    float GetClipX(AABB first, AABB second, float delta_x) const noexcept;

    float GetClipY(AABB first, AABB second, float delta_y) const noexcept;

    float GetClipZ(AABB first, AABB second, float delta_z) const noexcept;

    glm::vec3 GetClippedMovementVector(BlockQuery auto&& world_block_query, const Observer& observer, glm::vec3 movement_vector) const noexcept
    {
        const glm::ivec3 observer_block_coords = observer.BlockPos();
        const glm::ivec3 start_coords = { observer_block_coords.x - 1, observer_block_coords.y - 1, observer_block_coords.z - 1 };

        std::vector<glm::dvec3> neighbor_blocks_coords;

        for (int y = 0; y < static_cast<int>(std::ceil(observer.Height())) + 2; ++y)
        {
            for (int z = 0; z < static_cast<int>(std::ceil(observer.Depth())) + 2; ++z)
            {
                for (int x = 0; x < static_cast<int>(std::ceil(observer.Width())) + 2; ++x)
                {
                    const glm::ivec3 neighbor_block_coords = { start_coords.x + x, start_coords.y + y, start_coords.z + z };

                    if (!world_block_query(neighbor_block_coords).IsSolid())
                    {
                        continue;
                    }

                    const glm::dvec3 neighbor_block_coords = { static_cast<double>(neighbor_block_coords.x), static_cast<double>(neighbor_block_coords.y), static_cast<double>(neighbor_block_coords.z) };
                    neighbor_blocks_coords.push_back(neighbor_block_coords);
                }
            }
        }

        glm::vec3 clipped_movement_vector = movement_vector;
        glm::dvec3 observer_min_coords = observer.Pos();
        glm::dvec3 observer_max_coords = { observer_min_coords.x + observer.Width(), observer_min_coords.y + observer.Height(), observer_min_coords.z + observer.Depth() };

        for (glm::dvec3 neighbor_block_min_coord : neighbor_blocks_coords)
        {
            const glm::dvec3 neighbor_block_max_coord = { neighbor_block_min_coord.x + 1.0, neighbor_block_min_coord.y + 1.0, neighbor_block_min_coord.z + 1.0 };
            const float clipped_x = GetClipX(observer_min_coords, observer_max_coords, neighbor_block_min_coord, neighbor_block_max_coord, movement_vector.x);
            clipped_movement_vector.x = clipped_x > 0.0f ? std::fmin(clipped_x, clipped_movement_vector.x) : std::fmax(clipped_x, clipped_movement_vector.x);
        }

        observer_min_coords.x += clipped_movement_vector.x;
        observer_max_coords.x += clipped_movement_vector.x;

        for (glm::dvec3 neighbor_block_min_coord : neighbor_blocks_coords)
        {
            const glm::dvec3 neighbor_block_max_coord = { neighbor_block_min_coord.x + 1.0, neighbor_block_min_coord.y + 1.0, neighbor_block_min_coord.z + 1.0 };
            const float clipped_y = GetClipY(observer_min_coords, observer_max_coords, neighbor_block_min_coord, neighbor_block_max_coord, movement_vector.y);
            clipped_movement_vector.y = clipped_y > 0.0f ? std::fmin(clipped_y, clipped_movement_vector.y) : std::fmax(clipped_y, clipped_movement_vector.y);
        }

        observer_min_coords.y += clipped_movement_vector.y;
        observer_max_coords.y += clipped_movement_vector.y;

        for (glm::dvec3 neighbor_block_min_coord : neighbor_blocks_coords)
        {
            const glm::dvec3 neighbor_block_max_coord = { neighbor_block_min_coord.x + 1.0, neighbor_block_min_coord.y + 1.0, neighbor_block_min_coord.z + 1.0 };
            const float clipped_z = GetClipZ(observer_min_coords, observer_max_coords, neighbor_block_min_coord, neighbor_block_max_coord, movement_vector.z);
            clipped_movement_vector.z = clipped_z > 0.0f ? std::fmin(clipped_z, clipped_movement_vector.z) : std::fmax(clipped_z, clipped_movement_vector.z);
        }

        observer_min_coords.z += clipped_movement_vector.z;
        observer_max_coords.z += clipped_movement_vector.z;

        return clipped_movement_vector;
    }
};

#endif // COLLISION_SYSTEM_HPP