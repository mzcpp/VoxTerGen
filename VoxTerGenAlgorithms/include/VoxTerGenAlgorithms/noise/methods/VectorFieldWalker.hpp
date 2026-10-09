#ifndef VECTOR_FIELD_WALKER_HPP
#define VECTOR_FIELD_WALKER_HPP

#include "VoxTerGenAlgorithms/utils/Common.hpp"
#include "VoxTerGenAlgorithms/utils/Vec2.hpp"
#include "VoxTerGenAlgorithms/utils/Vec3.hpp"

#include <array>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <random>

template<typename ScalarField>
concept Field2D = requires(const ScalarField & f, double x, double y)
{
    { f.Sample(x, y) } -> std::convertible_to<double>;
};

template<typename ScalarField>
concept Field3D = requires(const ScalarField & f, double x, double y, double z)
{
    { f.Sample(x, y, z) } -> std::convertible_to<double>;
};

template <typename ScalarField>
class VectorFieldWalker
{
private:
    ScalarField scalar_field_;
    std::array<double, 9> offsets_;
    double step_size_;
    double frequency_;

public:
    VectorFieldWalker(const ScalarField& field, std::uint64_t seed, double step_size, double frequency) : scalar_field_(field), offsets_(0.0)
    {
        step_size_ = std::isfinite(step_size) && step_size > 0.0 ? step_size : 1.0;
        frequency_ = std::isfinite(frequency) && frequency > 0.0 ? frequency : 0.0025;

        constexpr double low_range = -1000.0;
        constexpr double high_range = 1000.0;

        std::mt19937_64 mt64(seed);
        std::uniform_real_distribution<> distrib(low_range, high_range);

        for (std::size_t i = 0; i < offsets_.size(); ++i)
        {
            offsets_[i] = distrib(mt64);
        }
    }

    std::optional<dvec2> Step(const dvec2& start_pos) const requires Field2D<ScalarField>
    {
        const double new_pos_dir_x = scalar_field_.Sample(
            (start_pos[0] + offsets_[0]) * frequency_, 
            (start_pos[1] + offsets_[1]) * frequency_
        );

        const double new_pos_dir_y = scalar_field_.Sample(
            (start_pos[0] + offsets_[2]) * frequency_, 
            (start_pos[1] + offsets_[3]) * frequency_
        );

        const math::Vec2 start = { start_pos[0], start_pos[1] };
        const math::Vec2 new_pos_dir = { new_pos_dir_x, new_pos_dir_y };
        
        if (!std::isfinite(new_pos_dir_x) || !std::isfinite(new_pos_dir_y) || new_pos_dir.LengthSquared() < 1e-6)
        {
            return std::nullopt;
        }

        const math::Vec2 new_pos = start + new_pos_dir.Normalized() * step_size_;

        return std::optional<dvec2>{ new_pos.x_, new_pos.y_ };
    }

    std::optional<dvec3> Step(const dvec3& start_pos) const requires Field3D<ScalarField>
    {
        const double new_pos_dir_x = scalar_field_.Sample(
            (start_pos[0] + offsets_[0]) * frequency_, 
            (start_pos[1] + offsets_[1]) * frequency_, 
            (start_pos[2] + offsets_[2]) * frequency_
        );
        
        const double new_pos_dir_y = scalar_field_.Sample(
            (start_pos[0] + offsets_[3]) * frequency_, 
            (start_pos[1] + offsets_[4]) * frequency_, 
            (start_pos[2] + offsets_[5]) * frequency_
        );
        
        const double new_pos_dir_z = scalar_field_.Sample(
            (start_pos[0] + offsets_[6]) * frequency_, 
            (start_pos[1] + offsets_[7]) * frequency_, 
            (start_pos[2] + offsets_[8]) * frequency_
        );

        const math::Vec3 start = { start_pos[0], start_pos[1], start_pos[2] };
        const math::Vec3 new_pos_dir = { new_pos_dir_x, new_pos_dir_y, new_pos_dir_z };

        if (!std::isfinite(new_pos_dir_x) || !std::isfinite(new_pos_dir_y) || !std::isfinite(new_pos_dir_z) || new_pos_dir.LengthSquared() < 1e-6)
        {
            return std::nullopt;
        }

        const math::Vec3 new_pos = start + new_pos_dir.Normalized() * step_size_;
        
        return std::optional<dvec3>{ new_pos.x_, new_pos.y_, new_pos.z_ };
    }
};

#endif