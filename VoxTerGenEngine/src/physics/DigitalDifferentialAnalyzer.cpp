#include "physics/DigitalDifferentialAnalyzer.hpp"

#include "core/Direction.hpp"

#include "utils/MathUtils.hpp"

#include "world/Block.hpp"

#include <glm/gtx/norm.hpp>
#include <glm/vec3.hpp>

#include <cmath>
#include <optional>
#include <limits>

std::optional<RaycastResult> DigitalDifferentialAnalyzer::CastRay(glm::dvec3 start_pos, glm::dvec3 ray_dir, double max_distance, BlockQuery auto&& world_block_query)
{
	if (glm::length2(ray_dir) <= std::numeric_limits<double>::epsilon())
	{
		return std::nullopt;
	}

	ray_dir = glm::normalize(ray_dir);

	const glm::dvec3 delta_distance(1.0 / std::fabs(ray_dir.x), 1.0 / std::fabs(ray_dir.y), 1.0 / std::fabs(ray_dir.z));
	const glm::ivec3 step_dir(math_utils::Sgn(ray_dir.x), math_utils::Sgn(ray_dir.y), math_utils::Sgn(ray_dir.z));

	glm::ivec3 current_block_coords(glm::floor(start_pos));
	glm::dvec3 ray_length(delta_distance);

	ray_length.x *= (ray_dir.x < 0) ? start_pos.x - current_block_coords.x : (current_block_coords.x + 1) - start_pos.x;
	ray_length.y *= (ray_dir.y < 0) ? start_pos.y - current_block_coords.y : (current_block_coords.y + 1) - start_pos.y;
	ray_length.z *= (ray_dir.z < 0) ? start_pos.z - current_block_coords.z : (current_block_coords.z + 1) - start_pos.z;

	RaycastResult raycast_result;
	double distance = 0.0;
	Direction last_face_hit = Direction::PosX;

	while (distance < max_distance)
	{
		if (ray_length.x <= ray_length.y && ray_length.x <= ray_length.z)
		{
			current_block_coords.x += step_dir.x;
			distance = ray_length.x;
			ray_length.x += delta_distance.x;
			last_face_hit = ToDirection(MajorAxis::X, step_dir.x == -1);
		}
		else if (ray_length.y <= ray_length.z)
		{
			current_block_coords.y += step_dir.y;
			distance = ray_length.y;
			ray_length.y += delta_distance.y;
			last_face_hit = ToDirection(MajorAxis::Y, step_dir.y == -1);
		}
		else
		{
			current_block_coords.z += step_dir.z;
			distance = ray_length.z;
			ray_length.z += delta_distance.z;
			last_face_hit = ToDirection(MajorAxis::Z, step_dir.z == -1);
		}

		const Block& block = world_block_query(current_block_coords).block_;

		if (block.IsSolid())
		{
			raycast_result.block_coords_ = current_block_coords;
			raycast_result.type_ = block.Type();
			raycast_result.face_ = last_face_hit;
			raycast_result.distance_ = distance;
			raycast_result.intersection_ = start_pos + ray_dir * distance;
			
			return std::optional<RaycastResult>(raycast_result);
		}
	}

	return std::nullopt;
}
