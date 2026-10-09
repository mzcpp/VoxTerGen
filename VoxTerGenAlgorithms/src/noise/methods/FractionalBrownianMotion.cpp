#include "pch.h"

#include "VoxTerGenAlgorithms/noise/methods/FractionalBrownianMotion.hpp"

FractionalBrownianMotion::FractionalBrownianMotion(Noise* noise, int octaves, double lacunarity, double persistence) :
    noise_(noise),
    signal_transform_(SignalTransform::NONE),
    octaves_(octaves),
    lacunarity_(lacunarity),
    persistence_(persistence)
{
}

double FractionalBrownianMotion::Sample(double x) const
{
    double result = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;
    double total_amplitude = 0.0;

    for (int octave = 0; octave < octaves_; ++octave)
    {
        result += amplitude * TransformSignal(noise_->Sample(x * frequency));

        frequency *= lacunarity_;
        total_amplitude += amplitude;
        amplitude *= persistence_;
    }

    return result / total_amplitude;
}

double FractionalBrownianMotion::Sample(double x, double y) const
{
    double result = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;
    double total_amplitude = 0.0;

    for (int octave = 0; octave < octaves_; ++octave)
    {
        result += amplitude * TransformSignal(noise_->Sample(x * frequency, y * frequency));

        frequency *= lacunarity_;
        total_amplitude += amplitude;
        amplitude *= persistence_;
    }

    return result / total_amplitude;
}

double FractionalBrownianMotion::Sample(double x, double y, double z) const
{
    double result = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;
    double total_amplitude = 0.0;

    for (int octave = 0; octave < octaves_; ++octave)
    {
        result += amplitude * TransformSignal(noise_->Sample(x * frequency, y * frequency, z * frequency));

        frequency *= lacunarity_;
        total_amplitude += amplitude;
        amplitude *= persistence_;
    }

    return result / total_amplitude;
}

double FractionalBrownianMotion::TransformSignal(double sample) const
{
    switch (signal_transform_)
    {
    case SignalTransform::NONE:
        return sample;
    case SignalTransform::ABS:
        return std::abs(sample);
    case SignalTransform::INVERT_ABS:
        return 1.0 - std::abs(sample);
    case SignalTransform::SQUARE:
        return sample * sample;
    case SignalTransform::RIDGED_SHAPE:
        sample = std::abs(sample);
        sample = 1.0 - sample;
        
        return sample * sample;
    }

    assert(false);
}