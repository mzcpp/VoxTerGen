#ifndef FRACTAL_BROWNIAN_MOTION_HPP
#define FRACTAL_BROWNIAN_MOTION_HPP

#include <concepts>
#include <cstdint>

enum class FractalType
{
    FBM, 
    RIDGED
};

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

    template <std::floating_point... Args>
    double GenerateFractal(Args... xyz) const noexcept
    {    
        const int octaves = 1;
        const double gain = 0.5; // persistance
        const double lacunarity = 2.0;

        double frequency = 1.0;
        double amplitude = 1.0;
        double result_noise = 0.0;
        
        for (int i = 0; i < octaves; ++i)
        {
            //result_noise += amplitude * noise_.Noise();

            frequency *= lacunarity;
            amplitude *= gain;
        }

        return result_noise;
    }
};

#endif // FRACTAL_BROWNIAN_MOTION_HPP

