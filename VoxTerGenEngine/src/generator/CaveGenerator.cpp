#include "generator/CaveGenerator.hpp"

#include "VoxTerGenAlgorithms/noise/core/Noise.hpp"
#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2FNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/OpenSimplex2SNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/PerlinNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/SimplexNoise.hpp"
#include "VoxTerGenAlgorithms/noise/core/WorleyNoise.hpp"

#include "VoxTerGenAlgorithms/noise/methods/VectorFieldWalker.hpp"

CaveGenerator::CaveGenerator(CaveType cave_type, std::uint64_t seed) : cave_type_(cave_type), seed_(seed)
{

}

void CaveGenerator::GenerateCarvedCave()
{

}


