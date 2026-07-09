#include "physics/DigitalDifferentialAnalyzer.hpp"

#include "utils/MathUtils.hpp"

#include "world/Block.hpp"

#include <glm/gtx/norm.hpp>
#include <glm/vec3.hpp>

#include <cmath>

RaycastResult DigitalDifferentialAnalyzer::CastRay(glm::dvec3 start_pos, glm::dvec3 ray_dir, BlockQuery auto&& world_block_query)
{
	ray_dir = glm::normalize(ray_dir);

	const glm::dvec3 step_size(1.0 / std::fabs(ray_dir.x), 1.0 / std::fabs(ray_dir.y), 1.0 / std::fabs(ray_dir.z));
	const glm::ivec3 step_dir(math_utils::Sgn(ray_dir.x), math_utils::Sgn(ray_dir.y), math_utils::Sgn(ray_dir.z));

	glm::ivec3 current_block_coords(start_pos);
	glm::dvec3 ray_length(step_size);

	ray_length.x *= (ray_dir.x < 0) ? start_pos.x - current_block_coords.x : 1 - start_pos.x - current_block_coords.x;
	ray_length.y *= (ray_dir.y < 0) ? start_pos.y - current_block_coords.y : 1 - start_pos.y - current_block_coords.y;
	ray_length.z *= (ray_dir.z < 0) ? start_pos.z - current_block_coords.z : 1 - start_pos.z - current_block_coords.z;

	constexpr double max_distance = 4.0;
	int block_distance = 0;
	double distance = 0.0;
	RaycastResult raycast_result;

	while (block_distance < max_distance)
	{
		if (ray_length.x < ray_length.y && ray_length.x < ray_length.z)
		{
			current_block_coords.x += step_dir.x;
			distance = ray_length.x;
			ray_length.x += step_size.x;
			raycast_result.face_ = ToDirection(MajorAxis::X, step_dir.x == 1);
		}
		else if (ray_length.y < ray_length.x && ray_length.y < ray_length.z)
		{
			current_block_coords.y += step_dir.y;
			distance = ray_length.y;
			ray_length.y += step_size.y;
			raycast_result.face_ = ToDirection(MajorAxis::Y, step_dir.y == 1);
		}
		else if (ray_length.z < ray_length.x && ray_length.z < ray_length.y)
		{
			current_block_coords.z += step_dir.z;
			distance = ray_length.z;
			ray_length.z += step_size.z;
			raycast_result.face_ = ToDirection(MajorAxis::Z, step_dir.z == 1);
		}

		++block_distance;

		const Block block = world_block_query(current_block_coords);

		if (block.IsSolid())
		{
			raycast_result.block_coords_ = current_block_coords;
			raycast_result.type_ = block.Type();
			raycast_result.intersection_ = start_pos + ray_dir * distance;
			break;
		}
	}

	return raycast_result;
}
