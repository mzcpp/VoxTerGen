#ifndef FRACTAL_BROWNIAN_MOTION_HPP
#define FRACTAL_BROWNIAN_MOTION_HPP

#include <concepts>

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

};

#endif // FRACTAL_BROWNIAN_MOTION_HPP

