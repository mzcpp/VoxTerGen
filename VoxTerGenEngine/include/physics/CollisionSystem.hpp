#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "physics/AABB.hpp"
#include "world/Observer.hpp"
#include "world/Block.hpp"

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <concepts>
#include <vector>
#include <cmath>

class Block;
class Observer;

class CollisionSystem
{   
private:

public:
    CollisionSystem();

    bool AABBIntersectsX(AABB first, AABB second) const noexcept;

    bool AABBIntersectsY(AABB first, AABB second) const noexcept;

    bool AABBIntersectsZ(AABB first, AABB second) const noexcept;

    bool AABBIntersects(AABB first, AABB second) const noexcept;

    double GetClipX(AABB first, AABB second, double delta_x) const noexcept;

    double GetClipY(AABB first, AABB second, double delta_y) const noexcept;

    double GetClipZ(AABB first, AABB second, double delta_z) const noexcept;

    glm::dvec3 GetClippedDisplacementVector(BlockQuery auto&& world_block_query, const Observer& observer, glm::dvec3 displacement_vector) const noexcept
    {
        if (glm::length2(displacement_vector) <= 0.0)
        {
            return displacement_vector;
        }
        
        const glm::ivec3 start_coords = observer.RelativeBlockPos(constants::observer::pos_offset) - glm::ivec3(1);
        constexpr int neighbor_radius = 5;
        std::vector<AABB> neighbor_blocks;

        for (int y = 0; y < static_cast<int>(std::ceil(observer.Height())) + neighbor_radius; ++y)
        {
            for (int z = 0; z < static_cast<int>(std::ceil(observer.Depth())) + neighbor_radius; ++z)
            {
                for (int x = 0; x < static_cast<int>(std::ceil(observer.Width())) + neighbor_radius; ++x)
                {
                    const BlockInfo neighbor_block_info = world_block_query(start_coords + glm::ivec3{ x, y, z });

                    if (!neighbor_block_info.block_.IsSolid())
                    {
                        continue;
                    }

                    const glm::dvec3 neighbor_block_min = {
                        static_cast<double>(neighbor_block_info.absolute_pos_.x), 
                        static_cast<double>(neighbor_block_info.absolute_pos_.y), 
                        static_cast<double>(neighbor_block_info.absolute_pos_.z)
                    };

                    const AABB neighbor_block(neighbor_block_min, neighbor_block_min + glm::dvec3(1.0));

                    neighbor_blocks.push_back(neighbor_block);
                }
            }
        }

        const glm::dvec3 observer_coords_offset = {
            static_cast<double>(observer.Width()),
            static_cast<double>(observer.Height()),
            static_cast<double>(observer.Depth())
        };

        glm::dvec3 clipped_movement_vector = displacement_vector;
        AABB observer_aabb = { observer.Pos(), observer.Pos(observer_coords_offset) };

        for (const AABB& neighbor_block : neighbor_blocks)
        {
            const double clipped_x = GetClipX(observer_aabb, neighbor_block, displacement_vector.x);
            clipped_movement_vector.x = displacement_vector.x > 0.0 ? std::fmin(clipped_x, clipped_movement_vector.x) : std::fmax(clipped_x, clipped_movement_vector.x);
        }

        observer_aabb.min_.x += clipped_movement_vector.x;
        observer_aabb.max_.x += clipped_movement_vector.x;

        for (const AABB& neighbor_block : neighbor_blocks)
        {
            const double clipped_y = GetClipY(observer_aabb, neighbor_block, displacement_vector.y);
            clipped_movement_vector.y = displacement_vector.y > 0.0 ? std::fmin(clipped_y, clipped_movement_vector.y) : std::fmax(clipped_y, clipped_movement_vector.y);
        }

        observer_aabb.min_.y += clipped_movement_vector.y;
        observer_aabb.max_.y += clipped_movement_vector.y;

        for (const AABB& neighbor_block : neighbor_blocks)
        {
            const double clipped_z = GetClipZ(observer_aabb, neighbor_block, displacement_vector.z);
            clipped_movement_vector.z = displacement_vector.z > 0.0 ? std::fmin(clipped_z, clipped_movement_vector.z) : std::fmax(clipped_z, clipped_movement_vector.z);
        }

        observer_aabb.min_.z += clipped_movement_vector.z;
        observer_aabb.max_.z += clipped_movement_vector.z;

        return clipped_movement_vector;
    }
};

#endif // COLLISION_SYSTEM_HPP