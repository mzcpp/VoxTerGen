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

    double Sample(double x) const;

    double Sample(double x, double y) const;

    double Sample(double x, double y, double z) const;
};

#endif // FRACTIONAL_BROWNIAN_MOTION_HPP

