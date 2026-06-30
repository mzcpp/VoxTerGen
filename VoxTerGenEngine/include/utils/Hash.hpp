#ifndef HASH_HPP
#define HASH_HPP

#include <glm/vec2.hpp>

#include <cstddef>
#include <functional>

namespace utils
{
	struct ivec2_hash
	{
		std::size_t operator()(glm::ivec2 vec) const noexcept
		{
			const std::size_t h1 = std::hash<int>{}(vec.x);
			const std::size_t h2 = std::hash<int>{}(vec.y);
			return h1 ^ (h2 << 1);
		}
	};
}

#endif // HASH_HPP