#include "physics/DigitalDifferentialAnalyzer.hpp"

#include "utils/MathUtils.hpp"

#include <glm/gtx/norm.hpp>
#include <glm/vec3.hpp>

RaycastResult DigitalDifferentialAnalyzer::CastRay(glm::dvec3 start_pos, glm::dvec3 dir, BlockQuery auto&& world_block_query)
{
	dir = glm::normalize(dir);

	const glm::dvec3 step_size(1.0 / dir.x, 1.0 / dir.y, 1.0 / dir.z);
	const glm::ivec3 step_dir = { Sgn(dir.x), Sgn(dir.y), Sgn(dir.z) };

	glm::ivec3 current_block_coords(start_pos);
	glm::dvec3 ray_length(0.0);

	// setup start

	const int max_block_distance = 4;
	int distance = 0;
	bool block_found = false;

	while (block_found && distance < max_block_distance)
	{
		if (ray_length.x < ray_length.y && ray_length.x < ray_length.z)
		{

		}
		else if (ray_length.y < ray_length.x && ray_length.y < ray_length.z)
		{
			
		}
		else if (ray_length.z < ray_length.x && ray_length.z < ray_length.y)
		{
			
		}
	}

	RaycastResult raycast_result;

	raycast_result.block_coords_ = current_block_coords;
	//raycast_result.type = block_found ?  : BlockType::Air;
	//raycast_result.face = ;
	//raycast_result.intersection = ;

	return raycast_result;
}
