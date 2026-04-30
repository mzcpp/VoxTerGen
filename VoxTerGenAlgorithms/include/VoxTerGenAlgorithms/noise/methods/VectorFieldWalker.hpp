#ifndef VECTOR_FIELD_WALKER_HPP
#define VECTOR_FIELD_WALKER_HPP

#include <array>
#include <cstdint>

#include "VoxTerGenAlgorithms/utils/Vec2.hpp"
#include "VoxTerGenAlgorithms/utils/Vec3.hpp"

template<typename Field>
concept FieldType = requires(Field field, double x, double y, double z)
{
    field.Sample(x);
    field.Sample(x, y);
    field.Sample(x, y, z);
};

using ivec2 = std::array<std::int64_t, 2>;
using ivec3 = std::array<std::int64_t, 3>;
using uivec2 = std::array<std::uint64_t, 2>;
using uivec3 = std::array<std::uint64_t, 3>;
using dvec2 = std::array<double, 2>;
using dvec3 = std::array<double, 3>;

template <FieldType Field>
class VectorFieldWalker
{
private:
    Field field_;

public:
    VectorFieldWalker(Field field) : field_(field)
    {

    }

    double Walk(double start_pos) const noexcept
    {
        return 0.0;
    }

    dvec2 Walk(const dvec2& start_pos) const noexcept
    {
        return {};
    }

    dvec3 Walk(const dvec3& start_pos) const noexcept
    {
        return {};
    }
};

#endif