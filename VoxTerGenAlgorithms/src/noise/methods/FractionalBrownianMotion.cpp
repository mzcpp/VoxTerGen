#include "pch.h"

#include "VoxTerGenAlgorithms/noise/methods/FractionalBrownianMotion.hpp"

#include "VoxTerGenAlgorithms/utils/Common.hpp"

double FractionalBrownianMotion::Sample(double x) const
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

double FractionalBrownianMotion::Sample(double x, double y) const
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

double FractionalBrownianMotion::Sample(double x, double y, double z) const
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