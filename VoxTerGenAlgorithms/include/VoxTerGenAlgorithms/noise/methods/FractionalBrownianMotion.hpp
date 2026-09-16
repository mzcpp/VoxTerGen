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
};

template <GenericNoise Noise>
class FractionalBrownianMotion
{
private:
    Noise noise_;
    SignalTransform signal_transform_;
    int octaves_;
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
        double total_amplitude = 0.0;

        for (int octave = 0; octave < octaves_; ++octave)
        {
            result += amplitude * TransformSignal(noise_.Sample(x * frequency));

            frequency *= lacunarity_;
            total_amplitude += amplitude;
            amplitude *= persistence_;
        }

        return result / total_amplitude;
    }

    double Sample(double x, double y) const
    {
        double result = 0.0;
        double frequency = 1.0;
        double amplitude = 1.0;
        double total_amplitude = 0.0;

        for (int octave = 0; octave < octaves_; ++octave)
        {
            result += amplitude * TransformSignal(noise_.Sample(x * frequency, y * frequency));

            frequency *= lacunarity_;
            total_amplitude += amplitude;
            amplitude *= persistence_;
        }

        return result / total_amplitude;
    }

    double Sample(double x, double y, double z) const
    {
        double result = 0.0;
        double frequency = 1.0;
        double amplitude = 1.0;
        double total_amplitude = 0.0;

        for (int octave = 0; octave < octaves_; ++octave)
        {
            result += amplitude * TransformSignal(noise_.Sample(x * frequency, y * frequency, z * frequency));

            frequency *= lacunarity_;
            total_amplitude += amplitude;
            amplitude *= persistence_;
        }

        return result / total_amplitude;
    }

    // Getters
    SignalTransform GetSignalTransform() const noexcept { return signal_transform_; }

    // Setters
    void SetSignalTransform(SignalTransform signal_transform) { signal_transform_ = signal_transform; }

private:
    double TransformSignal(double sample)
    {
        switch (signal_transform_)
        {
        case NONE:
            return sample;
        case ABS:
            return std::fabs(sample);
        case INVERT_ABS:
            return 1.0 - std::fabs(sample);
        case SQUARE:
            return sample * sample;
        case RIDGED_SHAPE:
        {
            sample = std::fabs(sample);
            sample = 1.0 - sample;
            return sample * sample;
        }
        }

        assert(false);
    }
};

#endif // FRACTIONAL_BROWNIAN_MOTION_HPP

