#ifndef VECTOR_FIELD_WALKER_HPP
#define VECTOR_FIELD_WALKER_HPP

#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <random>

#include "VoxTerGenAlgorithms/utils/Common.hpp"
#include "VoxTerGenAlgorithms/utils/Vec2.hpp"
#include "VoxTerGenAlgorithms/utils/Vec3.hpp"

template<typename Field>
concept Field2D = requires(const Field& f, double x, double y)
{
    { f.Sample(x, y) } -> std::convertible_to<double>;
};


template<typename Field>
concept Field3D = requires(const Field& f, double x, double y, double z)
{
    { f.Sample(x, y, z) } -> std::convertible_to<double>;
};

template <typename Field>
class VectorFieldWalker
{
private:
    std::uint64_t seed_;
    Field field_;
    std::array<double, 6> offsets_;
    double step_size_;

public:
    VectorFieldWalker(const Field& field, std::uint64_t seed, double step_size) : field_(field), seed_(seed), step_size_(step_size), offsets_(0.0)
    {
        constexpr double low_range = -1000.0;
        constexpr double high_range = 1000.0;

        std::mt19937_64 mt64(seed_);
        std::uniform_real_distribution<> distrib(low_range, high_range);

        for (std::size_t i = 0; i < offsets_.size(); ++i)
        {
            offsets_[i] = distrib(mt64);
        }
    }

    dvec2 Step(const dvec2& start_pos) const noexcept requires Field2D<Field>
    {
        const double new_pos_dir_x = field_.Sample(start_pos[0], start_pos[1]);
        const double new_pos_dir_y = field_.Sample(start_pos[0] + offsets_[0], start_pos[1] + offsets_[1]);

        const math::Vec2 start = { start_pos[0], start_pos[1] };
        const math::Vec2 new_pos_dir = { new_pos_dir_x, new_pos_dir_y };
        
        if (!std::isfinite(new_pos_dir_x) || !std::isfinite(new_pos_dir_y) || new_pos_dir.LengthSquared() < 1e-6)
        {
            return start_pos;
        }

        const math::Vec2 new_pos = start + new_pos_dir.Normalized() * step_size_;

        return { new_pos.x_, new_pos.y_ };
    }

    dvec3 Step(const dvec3& start_pos) const noexcept requires Field3D<Field>
    {
        const double new_pos_dir_x = field_.Sample(start_pos[0], start_pos[1], start_pos[2]);
        const double new_pos_dir_y = field_.Sample(start_pos[0] + offsets_[0], start_pos[1] + offsets_[1], start_pos[2] + offsets_[2]);
        const double new_pos_dir_z = field_.Sample(start_pos[0] + offsets_[3], start_pos[1] + offsets_[4], start_pos[2] + offsets_[5]);

        const math::Vec3 start = { start_pos[0], start_pos[1], start_pos[2] };
        const math::Vec3 new_pos_dir = { new_pos_dir_x, new_pos_dir_y, new_pos_dir_z };

        if (!std::isfinite(new_pos_dir_x) || !std::isfinite(new_pos_dir_y) || !std::isfinite(new_pos_dir_z) || new_pos_dir.LengthSquared() < 1e-6)
        {
            return start_pos;
        }

        const math::Vec3 new_pos = start + new_pos_dir.Normalized() * step_size_;
        
        return { new_pos.x_, new_pos.y_, new_pos.z_ };
    }
};

#endif