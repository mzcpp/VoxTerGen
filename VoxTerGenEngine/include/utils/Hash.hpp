#ifndef HASH_HPP
#define HASH_HPP

namespace utils
{
	struct ivec2_hash
	{
		std::size_t operator()(const glm::ivec2& vec) const noexcept
		{
			const std::size_t h1 = std::hash<int>{}(vec.x);
			const std::size_t h2 = std::hash<int>{}(vec.y);
			return h1 ^ (h2 << 1);
		}
	};
}

#endif // HASH_HPP