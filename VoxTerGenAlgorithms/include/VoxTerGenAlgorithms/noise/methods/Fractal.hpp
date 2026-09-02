#ifndef FRACTAL_HPP
#define FRACTAL_HPP

#include <concepts>
#include <cstdint>
#include <tuple>
#include <cmath>
#include <algorithm>

template<typename Noise>
concept NoiseType = requires(Noise noise, double x, double y, double z)
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

    template<NoiseType Noise>
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

//template<typename DomainTransform>
//concept DomainTransformType = std::invocable<DomainTransform, double>;

//template<typename SignalTransform>
//concept SignalTransformType = std::invocable<SignalTransform, double>;

//template<typename Accumulation>
//concept AccumulationType = std::invocable<Accumulation, double, double, double>;

template<NoiseType Noise, typename DomainTransform, typename SignalTransform, typename Accumulator>
class Fractal
{
private:
    Noise noise_;
    DomainTransform domain_transform_;
    SignalTransform signal_transform_;
    Accumulator accumulator_;

    int octaves_;
    double amplitude_decay_; // gain/persistence
    double lacunarity_;

public:
    Fractal(
        const Noise& noise,
        const DomainTransform& domain,
        const SignalTransform& signal,
        const Accumulator& acc,
        int octaves,
        double amplitude_decay,
        double lacunarity)
        :
        noise_(noise),
        domain_transform_(domain),
        signal_transform_(signal),
        accumulator_(acc),
        octaves_(octaves),
        amplitude_decay_(amplitude_decay),
        lacunarity_(lacunarity)
    {
    }

    template<std::floating_point... Args>
    double GenerateFractal(Args... xyz) noexcept
    {
        accumulator_.Reset();

        double frequency = 1.0;
        double amplitude = 1.0;
        double result_noise = 0.0;
        auto warped_coords = domain_transform_(xyz...);

        for (int octave = 0; octave < octaves_; ++octave)
        {
            double signal = std::apply([&](auto... coord) {
                return noise_.Sample((coord * frequency)...);
                }, warped_coords);

            signal = signal_transform_(signal);

            result_noise = accumulator_(result_noise, signal, amplitude);

            frequency *= lacunarity_;
            amplitude *= amplitude_decay_;
        }

        return result_noise;
    }
};

#endif // FRACTAL_HPP

