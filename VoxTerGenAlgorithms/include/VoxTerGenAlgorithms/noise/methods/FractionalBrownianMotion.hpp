#ifndef FRACTIONAL_BROWNIAN_MOTION_HPP
#define FRACTIONAL_BROWNIAN_MOTION_HPP

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"

#include "VoxTerGenAlgorithms/utils/Common.hpp"

#include <cmath>
#include <cassert>

enum class SignalTransform
{
    NONE,
    ABS,
    INVERT_ABS,
    SQUARE,
    RIDGED_SHAPE
};

class FractionalBrownianMotion
{
private:
    Noise* noise_;
    SignalTransform signal_transform_;
    int octaves_;
    double lacunarity_;
    double persistence_;

public:
    FractionalBrownianMotion() = default;

    FractionalBrownianMotion(Noise* noise, int octaves, double lacunarity, double persistence);

    double Sample(double x) const;

    double Sample(double x, double y) const;

    double Sample(double x, double y, double z) const;
    
    // Getters
    SignalTransform GetSignalTransform() const noexcept { return signal_transform_; }

    // Setters
    void SetSignalTransform(SignalTransform signal_transform) { signal_transform_ = signal_transform; }

private:
    double TransformSignal(double sample) const;
};

#endif // FRACTIONAL_BROWNIAN_MOTION_HPP

