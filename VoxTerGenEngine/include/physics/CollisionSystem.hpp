#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "physics/AABB.hpp"
#include "world/Observer.hpp"
#include "world/Block.hpp"

#include "glm/vec3.hpp"

#include <concepts>
#include <vector>
#include <cmath>
#include <iostream>

class Block;
class Observer;

template <typename Fnc>
concept BlockQuery = std::invocable<Fnc, glm::ivec3> && std::convertible_to<std::invoke_result_t<Fnc, glm::ivec3>, BlockInfo>;

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

    glm::vec3 GetClippedDisplacementVector(BlockQuery auto&& world_block_query, const Observer& observer, glm::vec3 displacement_vector) const noexcept
    {
        const glm::ivec3 observer_block_coords = observer.RelativeBlockPos();
        const glm::ivec3 start_coords = { observer_block_coords.x - 1, observer_block_coords.y - 1, observer_block_coords.z - 1 };

        std::vector<glm::dvec3> neighbor_blocks_coords;
        std::cout << "NEIGHBORS\n";

        for (int y = 0; y < static_cast<int>(std::ceil(observer.Height())) + 4; ++y)
        {
            for (int z = 0; z < static_cast<int>(std::ceil(observer.Depth())) + 4; ++z)
            {
                for (int x = 0; x < static_cast<int>(std::ceil(observer.Width())) + 4; ++x)
                {
                    const glm::ivec3 neighbor_block_coords = { start_coords.x + x, start_coords.y + y, start_coords.z + z };

                    const BlockInfo neighbor_block_info = world_block_query(neighbor_block_coords);

                    if (!neighbor_block_info.block_.IsSolid())
                    {
                        continue;
                    }

                    std::cout << neighbor_block_info.absolute_pos_.x << ' ' << neighbor_block_info.absolute_pos_.y << ' ' << neighbor_block_info.absolute_pos_.z << '\n';
                    neighbor_blocks_coords.push_back({ static_cast<double>(neighbor_block_info.absolute_pos_.x), static_cast<double>(neighbor_block_info.absolute_pos_.y), static_cast<double>(neighbor_block_info.absolute_pos_.z) });
                }
            }
        }

        const glm::dvec3 observer_min_coords = observer.Pos();
        const glm::dvec3 observer_max_coords =
        {
            observer_min_coords.x + static_cast<double>(observer.Width()),
            observer_min_coords.y + static_cast<double>(observer.Height()),
            observer_min_coords.z + static_cast<double>(observer.Depth())
        };

        glm::vec3 clipped_movement_vector = displacement_vector;
        AABB observer_aabb = { observer_min_coords, observer_max_coords };
        
        //if (!neighbor_blocks_coords.empty())
        //{
        //    glm::dvec3 first = neighbor_blocks_coords.front();
        //    bool intersects = AABBIntersects(observer_aabb, { first, { first.x + 1.0, first.y + 1.0, first.z + 1.0 } });


        //    std::cout << (intersects ? "COLLISION" : "NO COLLISION") << '\n';
        //}

        //return displacement_vector;

        for (glm::dvec3 neighbor_block_min_coord : neighbor_blocks_coords)
        {
            const glm::dvec3 neighbor_block_max_coord = { neighbor_block_min_coord.x + 1.0, neighbor_block_min_coord.y + 1.0, neighbor_block_min_coord.z + 1.0 };

            const AABB neighbor_block_aabb = { neighbor_block_min_coord, neighbor_block_max_coord };
            const float clipped_x = GetClipX(observer_aabb, neighbor_block_aabb, displacement_vector.x);
            clipped_movement_vector.x = clipped_x > 0.0f ? std::fmin(clipped_x, clipped_movement_vector.x) : std::fmax(clipped_x, clipped_movement_vector.x);
        }

        observer_aabb.min_.x += clipped_movement_vector.x;
        observer_aabb.max_.x += clipped_movement_vector.x;

        std::cout << "vector " << clipped_movement_vector.x << ' ' << clipped_movement_vector.y << ' ' << clipped_movement_vector.z << '\n';

        for (glm::dvec3 neighbor_block_min_coord : neighbor_blocks_coords)
        {
            const glm::dvec3 neighbor_block_max_coord = { neighbor_block_min_coord.x + 1.0, neighbor_block_min_coord.y + 1.0, neighbor_block_min_coord.z + 1.0 };
            const AABB neighbor_block_aabb = { neighbor_block_min_coord, neighbor_block_max_coord };
            const float clipped_y = GetClipY(observer_aabb, neighbor_block_aabb, displacement_vector.y);
            clipped_movement_vector.y = clipped_y > 0.0f ? std::fmin(clipped_y, clipped_movement_vector.y) : std::fmax(clipped_y, clipped_movement_vector.y);
        }

        observer_aabb.min_.y += clipped_movement_vector.y;
        observer_aabb.max_.y += clipped_movement_vector.y;

        for (glm::dvec3 neighbor_block_min_coord : neighbor_blocks_coords)
        {
            const glm::dvec3 neighbor_block_max_coord = { neighbor_block_min_coord.x + 1.0, neighbor_block_min_coord.y + 1.0, neighbor_block_min_coord.z + 1.0 };
            const AABB neighbor_block_aabb = { neighbor_block_min_coord, neighbor_block_max_coord };
            const float clipped_z = GetClipZ(observer_aabb, neighbor_block_aabb, displacement_vector.z);
            clipped_movement_vector.z = clipped_z > 0.0f ? std::fmin(clipped_z, clipped_movement_vector.z) : std::fmax(clipped_z, clipped_movement_vector.z);
        }

        observer_aabb.min_.z += clipped_movement_vector.z;
        observer_aabb.max_.z += clipped_movement_vector.z;

        return clipped_movement_vector;
    }
};

#endif // COLLISION_SYSTEM_HPP