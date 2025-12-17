#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include "utils/Constants.hpp"

#include <cmath>
#include <limits>
#include <type_traits>

template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool FloatingPointNearZero(T a, T epsilon = static_cast<T>(Constants::Math::float_epsilon))
{
    return std::fabs(a) <= epsilon;
}

template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool FloatingPointSame(T a, T b, T epsilon = static_cast<T>(Constants::Math::float_epsilon))
{
    return std::fabs(a - b) <= std::fmax(std::fabs(a), std::fabs(b)) * epsilon;
}

template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool FloatingPointGreaterThan(T a, T b, T epsilon = static_cast<T>(Constants::Math::float_epsilon))
{
    return (a - b) > std::fmax(std::fabs(a), std::fabs(b)) * epsilon;
}

template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool FloatingPointLessThan(T a, T b, T epsilon = static_cast<T>(Constants::Math::float_epsilon))
{
    return (b - a) > std::fmax(std::fabs(a), std::fabs(b)) * epsilon;
}

template <typename T>
inline constexpr int Sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

inline bool Vec3Equal(const glm::vec3& a, const glm::vec3& b, float eps = Constants::Math::float_epsilon)
{
    return glm::length(a - b) < eps;
}

#endif
