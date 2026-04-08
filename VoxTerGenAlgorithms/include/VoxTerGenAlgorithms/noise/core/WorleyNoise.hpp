#ifndef WORLEY_NOISE_HPP
#define WORLEY_NOISE_HPP

#include <cstdint>

struct Point
{
	double x = 0.0;
	double y = 0.0;
};

class WorleyNoise
{
private:
	std::uint64_t seed_;
	int cell_size_;

public:
	WorleyNoise(std::uint64_t seed, int cell_size);

	double Noise(double x) const;

	double Noise(double x, double y) const;

	double Noise(double x, double y, double z) const;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }

private:

	Point GenRandomPoint() const noexcept;
};

#endif // WORLEY_NOISE_HPP

