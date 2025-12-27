#ifndef MESH_HPP
#define MESH_HPP

#include "mesh/Vertex.hpp"

#include <vector>
#include <cstdint>

class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

public:

};

#endif // MESH_HPP

