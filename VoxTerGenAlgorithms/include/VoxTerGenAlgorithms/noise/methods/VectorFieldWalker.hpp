#ifndef VECTOR_FIELD_WALKER_HPP
#define VECTOR_FIELD_WALKER_HPP

template<typename Noise>
concept NoiseType = requires(Noise noise, double x, double y, double z)
{
    noise.Noise(x);
    noise.Noise(x, y);
    noise.Noise(x, y, z);
};

template <NoiseType GradientNoise>
class VectorFieldWalker
{
private:

public:

};

#endif