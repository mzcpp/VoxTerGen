#ifndef MESH_HPP
#define MESH_HPP

#include "mesh/Vertex.hpp"

#include <vector>
#include <cstdint>

class Mesh
{
private:
	std::vector<Vertex> vertices_;
	std::vector<uint32_t> indices_;

public:
	std::vector<Vertex>& Vertices() { return vertices_; }
	std::vector<uint32_t>& Indices() { return indices_; }
};

#endif // MESH_HPP

