#ifndef FRACTAL_BROWNIAN_MOTION_HPP
#define FRACTAL_BROWNIAN_MOTION_HPP

#include <concepts>
#include <cstdint>

template <typename Noise>
concept NoiseType = requires(Noise noise, double x, double y, double z)
{
    noise.Noise(x);
    noise.Noise(x, y);
    noise.Noise(x, y, z);
};

template <NoiseType Noise>
class FractalBrownianMotion
{
private:
    Noise noise_;

public:
    FractalBrownianMotion(Noise noise) : noise_(noise)
    {

    }

    double GenerateFractal(double x) const noexcept
    {
        return 0.0;
    }

    double GenerateFractal(double x, double y) const noexcept
    {
        return 0.0;
    }

    double GenerateFractal(double x, double y, double z) const noexcept
    {
        return 0.0;
    }

};

#endif // FRACTAL_BROWNIAN_MOTION_HPP

