#ifndef FRACTAL_HPP
#define FRACTAL_HPP

#include <concepts>
#include <cstdint>
#include <tuple>
#include <cmath>
#include <algorithm>

// template<NoiseType Noise, typename DomainTransform, typename SignalTransform, typename Accumulator>
// class Fractal
// {
// private:
//     Noise noise_;
//     DomainTransform domain_transform_;
//     SignalTransform signal_transform_;
//     Accumulator accumulator_;

//     int octaves_;
//     double amplitude_decay_; // gain/persistence
//     double lacunarity_;

// public:
//     Fractal(
//         const Noise& noise,
//         const DomainTransform& domain,
//         const SignalTransform& signal,
//         const Accumulator& acc,
//         int octaves,
//         double amplitude_decay,
//         double lacunarity)
//         :
//         noise_(noise),
//         domain_transform_(domain),
//         signal_transform_(signal),
//         accumulator_(acc),
//         octaves_(octaves),
//         amplitude_decay_(amplitude_decay),
//         lacunarity_(lacunarity)
//     {
//     }

//     template<std::floating_point... Args>
//     double GenerateFractal(Args... xyz) noexcept
//     {
//         accumulator_.Reset();

//         double frequency = 1.0;
//         double amplitude = 1.0;
//         double result_noise = 0.0;
//         auto warped_coords = domain_transform_(xyz...);

//         for (int octave = 0; octave < octaves_; ++octave)
//         {
//             double signal = std::apply([&](auto... coord) {
//                 return noise_.Sample((coord * frequency)...);
//                 }, warped_coords);

//             signal = signal_transform_(signal);

//             result_noise = accumulator_(result_noise, signal, amplitude);

//             frequency *= lacunarity_;
//             amplitude *= amplitude_decay_;
//         }

//         return result_noise;
//     }
// };

#endif // FRACTAL_HPP

