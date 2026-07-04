#ifndef DIGITAL_DIFFERENTIAL_ANALYZER_HPP
#define DIGITAL_DIFFERENTIAL_ANALYZER_HPP

#include "core/Direction.hpp"

#include "world/Block.hpp"

#include <glm/vec3.hpp>

struct RaycastResult
{
	glm::ivec3 block_coords_;
	BlockType type_;
	Direction face_;
	glm::dvec3 intersection_;
};

template <typename Fnc>
concept BlockQuery = std::invocable<Fnc, glm::ivec3>&& std::convertible_to<std::invoke_result_t<Fnc, glm::ivec3>, BlockInfo>;

class DigitalDifferentialAnalyzer
{
private:

public:
	static RaycastResult CastRay(glm::dvec3 start_pos, glm::dvec3 dir, BlockQuery auto&& world_block_query);
};

#endif // DIGITAL_DIFFERENTIAL_ANALYZER_HPP

