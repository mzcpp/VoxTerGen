#ifndef COMMON_HPP
#define COMMON_HPP

#include <array>
#include <concepts>
#include <cstdint>
#include <tuple>
#include <cmath>
#include <algorithm>

//namespace vtg
//{
	using ivec2 = std::array<std::int64_t, 2>;
	using ivec3 = std::array<std::int64_t, 3>;
	using ivec4 = std::array<std::int64_t, 4>;

	using uivec2 = std::array<std::uint64_t, 2>;
	using uivec3 = std::array<std::uint64_t, 3>;
	using uivec4 = std::array<std::uint64_t, 4>;

	using dvec2 = std::array<double, 2>;
	using dvec3 = std::array<double, 3>;
	using dvec4 = std::array<double, 4>;
//} // namespace vtg

template<typename Noise>
concept GenericNoise = requires(Noise noise, double x, double y, double z)
{
    noise.Sample(x);
    noise.Sample(x, y);
    noise.Sample(x, y, z);
};

namespace domain_transform
{
    struct None
    {
        template<typename... Args>
        auto operator()(Args... coords) const
        {
            return std::tuple<Args...>(coords...);
        }
    };

    template<GenericNoise Noise>
    struct DomainWarp
    {
        DomainWarp(Noise warp_noise, double strength) : warp_noise_(warp_noise), strength_(strength)
        {
        }

        Noise warp_noise_;
        double strength_;

        template<typename... Args>
        auto operator()(Args... coords) const
        {
            // TODO: warp each axis separately
            const double offset_coords = warp_noise_.Sample(coords...) * strength_;
            return std::tuple<Args...>((coords + offset_coords)...);
        }
    };
} // namespace domain_transform

namespace signal_transform
{
    struct None { double operator()(double n) const { return n; } };
    struct Abs { double operator()(double n) const { return std::abs(n); } };
    struct InvertAbs { double operator()(double n) const { return 1.0 - std::abs(n); } };
    struct Square { double operator()(double n) const { return n * n; } };

    struct RidgedShape
    {
        double operator()(double n) const
        {
            n = std::abs(n);
            n = 1.0 - n;
            n = n * n;
            return n;
        }
    };
}  // namespace domain_transform

namespace accumulation
{
    struct Additive
    {
        double operator()(double result_sum, double signal, double amplitude) const
        {
            return result_sum + signal * amplitude;
        }

        void Reset() {}
    };

    struct RidgedFBM
    {
        double weight_;
        double ridge_gain_;

        RidgedFBM(double ridge_gain) : weight_(1.0), ridge_gain_(ridge_gain)
        {
        }

        double operator()(double result_sum, double signal, double amplitude)
        {
            signal *= weight_;
            weight_ = std::clamp(signal * ridge_gain_, 0.0, 1.0);
            return result_sum + signal * amplitude;
        }

        void Reset()
        {
            weight_ = 1.0;
        }
    };
} // namespace accumulation

#endif