#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "world/Observer.hpp"
#include "world/Block.hpp"
//#include "AABB.hpp"

#include "glm/vec3.hpp"

#include <concepts>
#include <vector>

struct AABB
{
    glm::dvec3 min_;
    glm::dvec3 max_;

    AABB(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z)
    {
        min_ = { min_x, min_y, min_z };
        max_ = { max_x, max_y, max_z };
    }

    AABB Expand(glm::dvec3 value)
    {
        AABB result;

        if (value.x > 0.0)
        {
            result.max_.x = max_.x + value.x;
        }
        else
        {
            result.min_.x = min_.x + value.x;
        }

        if (value.y > 0.0)
        {
            result.max_.y = max_.y + value.y;
        }
        else
        {
            result.min_.y = min_.y + value.y;
        }

        if (value.z > 0.0)
        {
            result.max_.z = max_.z + value.z;
        }
        else
        {
            result.min_.z = min_.z + value.z;
        }

        return result;
    }

    AABB Grow(glm::dvec3 value)
    {
        AABB result;

        result.max_.x = max_.x + value.x;
        result.max_.y = max_.y + value.y;
        result.max_.z = max_.z + value.z;

        result.min_.x = min_.x - value.x;      
        result.min_.y = min_.y - value.y;
        result.min_.z = min_.z - value.z;

        return result;
    }

    void Move(glm::dvec3 vec)
    {
        min_.x += vec.x;
        min_.y += vec.y;
        min_.z += vec.z;
    }
};

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
        const glm::dvec3 observer_min_coords = observer.Pos();
        const glm::dvec3 observer_max_coords = { observer_min_coords.x + observer.Width(), observer_min_coords.y + observer.Height(), observer_min_coords.z + observer.Depth() };
        const glm::ivec3 observer_block_coords = observer.BlockPos();
        const glm::ivec3 start_coords = { observer_block_coords.x - 1, observer_block_coords.y - 1, observer_block_coords.z - 1 };

        std::vector<glm::dvec3> neighbor_blocks_coords;

        for (int y = 0; y < static_cast<int>(observer.Height()) + 2; ++y)
        {
            for (int z = 0; z < static_cast<int>(observer.Depth()) + 2; ++z)
            {
                for (int x = 0; x < static_cast<int>(observer.Width()) + 2; ++x)
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

