#include "generator/TerrainGenerator.hpp"

TerrainGenerator::TerrainGenerator(NoiseType noise_type, std::uint64_t seed) : seed_(seed), noise_type_(noise_type)
{

}

double TerrainGenerator::GetCoordHeight(double x, double y)
{
    // which frequency for stupid simple height map?

    

    return 0.0;
}