#ifndef NOISE_HPP
#define NOISE_HPP

#include <cstdint>

class Noise
{
protected:
	std::uint64_t seed_;

public:
	Noise(std::uint64_t seed) : seed_(seed)
	{
	}

	virtual ~Noise() = default;

	virtual double Sample(double x) const noexcept = 0;

	virtual double Sample(double x, double y) const noexcept = 0;

	virtual double Sample(double x, double y, double z) const noexcept = 0;

	virtual double Sample(double x, double y, double z, double w) const noexcept = 0;

	// Getters
	std::uint64_t Seed() const noexcept { return seed_; }
};

#endif // NOISE_HPP

