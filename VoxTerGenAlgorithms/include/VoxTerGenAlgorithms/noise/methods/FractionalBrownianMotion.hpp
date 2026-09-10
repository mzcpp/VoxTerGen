#ifndef FRACTIONAL_BROWNIAN_MOTION_HPP
#define FRACTIONAL_BROWNIAN_MOTION_HPP

#include "VoxTerGenAlgorithms/utils/Common.hpp"

enum class SignalTransform
{
    NONE, 
    ABS, 
    INVERT_ABS, 
    SQUARE, 
    RIDGED_SHAPE
}

template <GenericNoise Noise>
class FractionalBrownianMotion
{
private:
    Noise noise_;
    SignalTransform signal_transform_;
    int octaves_;
    double frequency_;
    double amplitude_;
    double lacunarity_;
    double persistence_;

public:
    FractionalBrownianMotion(
        Noise noise, 
        int octaves, 
        double lacunarity, 
        double persistence)
        : 
        noise_(noise), 
        signal_transform_(SignalTransform::NONE), 
        octaves_(octaves), 
        lacunarity_(lacunarity), 
        persistence_(persistence)
    {
    }

    double Sample(double x) const
    {
        double result = 0.0;
        double frequency = 1.0;
        double amplitude = 1.0;

        for (int octave = 0; octave < octaves_; ++octave)
        {
            result += amplitude * noise_.Sample(x * frequency);

            frequency *= lacunarity_;
            amplitude *= persistence_;
        }

        return result;
    }

    double Sample(double x, double y) const
    {
        double result = 0.0;
        double frequency = 1.0;
        double amplitude = 1.0;

        for (int octave = 0; octave < octaves_; ++octave)
        {
            result += amplitude * noise_.Sample(x * frequency, y * frequency);

            frequency *= lacunarity_;
            amplitude *= persistence_;
        }

        return result;
    }

    double Sample(double x, double y, double z) const
    {
        double result = 0.0;
        double frequency = 1.0;
        double amplitude = 1.0;

        for (int octave = 0; octave < octaves_; ++octave)
        {
            result += amplitude * noise_.Sample(x * frequency, y * frequency, z * frequency);

            frequency *= lacunarity_;
            amplitude *= persistence_;
        }

        return result;
    }
};

#endif // FRACTIONAL_BROWNIAN_MOTION_HPP

