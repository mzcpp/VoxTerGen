#ifndef DIGITAL_DIFFERENTIAL_ANALYZER_HPP
#define DIGITAL_DIFFERENTIAL_ANALYZER_HPP

#include "core/Direction.hpp"

#include "world/Block.hpp"

#include <glm/vec3.hpp>

#include <optional>

struct RaycastResult
{
	glm::ivec3 block_coords_;
	BlockType type_;
	Direction face_;
	double distance_;
	glm::dvec3 intersection_;
};

class DigitalDifferentialAnalyzer
{
private:

public:
	static std::optional<RaycastResult> CastRay(glm::dvec3 start_pos, glm::dvec3 dir, double max_distance, BlockQuery auto&& world_block_query);
};

#endif // DIGITAL_DIFFERENTIAL_ANALYZER_HPP

