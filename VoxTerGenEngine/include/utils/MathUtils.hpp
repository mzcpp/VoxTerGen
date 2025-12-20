#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include "utils/Constants.hpp"

#include <glm/glm.hpp>

#include <cmath>
#include <limits>
#include <type_traits>

/**
 * @brief Checks if a floating-point number is approximately zero.
 *
 * @tparam T Floating-point type
 * @param a Value to check
 * @param epsilon Tolerance (default: Constants::Math::float_epsilon)
 * 
 * @return true if |a| <= epsilon, false otherwise
 */
template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool FloatingPointNearZero(T a, T epsilon = static_cast<T>(Constants::Math::float_epsilon))
{
    return std::fabs(a) <= epsilon;
}

/**
 * @brief Checks if two floating-point numbers are approximately equal.
 *
 * @tparam T Floating-point type
 * @param a First value
 * @param b Second value
 * @param epsilon Relative tolerance (default: Constants::Math::float_epsilon)
 * 
 * @return true if the numbers are considered equal, false otherwise
 */
template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool FloatingPointSame(T a, T b, T epsilon = static_cast<T>(Constants::Math::float_epsilon))
{
    return std::fabs(a - b) <= std::fmax(std::fabs(a), std::fabs(b)) * epsilon;
}

/**
 * @brief Checks if one floating-point number is significantly greater than another.
 *
 * @tparam T Floating-point type
 * @param a First value
 * @param b Second value
 * @param epsilon Relative tolerance (default: Constants::Math::float_epsilon)
 * 
 * @return true if a > b by more than epsilon, false otherwise
 */
template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool FloatingPointGreaterThan(T a, T b, T epsilon = static_cast<T>(Constants::Math::float_epsilon))
{
    return (a - b) > std::fmax(std::fabs(a), std::fabs(b)) * epsilon;
}

/**
 * @brief Checks if one floating-point number is significantly less than another.
 *
 * @tparam T Floating-point type
 * @param a First value
 * @param b Second value
 * @param epsilon Relative tolerance (default: Constants::Math::float_epsilon)
 * 
 * @return true if a < b by more than epsilon, false otherwise
 */
template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool FloatingPointLessThan(T a, T b, T epsilon = static_cast<T>(Constants::Math::float_epsilon))
{
    return (b - a) > std::fmax(std::fabs(a), std::fabs(b)) * epsilon;
}

/**
 * @brief Returns the sign of a value.
 *
 * @tparam T Numeric type
 * @param val Value to check
 * 
 * @return -1 if val < 0, 0 if val == 0, 1 if val > 0
 */
template <typename T>
inline constexpr int Sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

/**
 * @brief Checks if two 3D vectors are approximately equal.
 *
 * @param a First vector
 * @param b Second vector
 * @param eps Tolerance (default: Constants::Math::float_epsilon)
 * 
 * @return true if vectors are approximately equal, false otherwise
 */
inline bool Vec3Equal(const glm::vec3& a, const glm::vec3& b, float eps = Constants::Math::float_epsilon)
{
    return glm::length(a - b) < eps;
}

#endif // MATH_UTILS_HPP