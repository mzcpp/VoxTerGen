#include "physics/DigitalDifferentialAnalyzer.hpp"

#include <glm/vec3.hpp>

RaycastResult DigitalDifferentialAnalyzer::CastRay(glm::dvec3 start_pos, glm::dvec3 dir, BlockQuery auto&& world_block_query)
{
	const glm::dvec3 step_size(1.0 / dir.x, 1.0 / dir.y, 1.0 / dir.z);
	return {};
}
