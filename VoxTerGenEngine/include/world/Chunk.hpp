#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "world/Block.hpp"
#include "utils/Constants.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <functional>
#include <memory>

/**
 * @brief Represents the six cardinal directions for neighboring blocks in a chunk.
 *
 * Used to query adjacent blocks in each axis-aligned direction.
 */
enum class Direction
{
	PosX = 0, 
	NegX, 
	PosY, 
	NegY, 
	PosZ, 
	NegZ
};

/**
 * @brief Represents a 3D chunk of blocks.
 *
 * Encapsulates a fixed-size array of blocks and provides
 * methods to access blocks by index or position, as well as
 * querying neighbors within the same chunk and filling 
 * the chunk with custom logic.
 */
class Chunk
{
private:
	static constexpr std::array<glm::ivec3, 6> neighbor_offsets_
	{
		glm::ivec3{ 1, 0, 0 }, glm::ivec3{ -1, 0, 0 },
		glm::ivec3{ 0, 1, 0 }, glm::ivec3{ 0, -1, 0 },
		glm::ivec3{ 0, 0, 1 }, glm::ivec3{ 0, 0, -1 }
	};

	std::array<Block, Constants::Chunk::size> chunk_;
	glm::ivec2 world_coords_;

public:
	/**
	 * @brief Constructs a chunk at the specified coordinates.
     * 
	 * @param chunk_coords The coordinates of the chunk in chunk space
	 */
	explicit Chunk(glm::ivec2 world_coords);

    /**
     * @brief Returns a reference to the block at the given coordinates.
     *
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @param check_index If true, validates the coordinates and throws on invalid access
     *
     * @return Reference to the block at (x, y, z)
     *
     * @throws std::out_of_range if check_index is true and coordinates are invalid
     */
    Block& BlockAt(int x, int y, int z, bool check_index = false);

    /**
     * @brief Returns a const reference to the block at the given coordinates.
     *
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @param check_index If true, validates the coordinates and throws on invalid access
     *
     * @return Const reference to the block at (x, y, z)
     *
     * @throws std::out_of_range if check_index is true and coordinates are invalid
     */
    const Block& BlockAt(int x, int y, int z, bool check_index = false) const;

    /**
     * @brief Returns the 3D coordinates of a block from its linear index.
     *
     * @param index Linear index of the block
     *
     * @return 3D coordinates of the block
     *
     * @throws std::out_of_range if index is outside valid range
     */
    glm::ivec3 Pos(int index) const;

    /**
     * @brief Checks if the provided block coordinates are valid.
     *
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     *
     * @return True if coordinates are inside chunk bounds, false otherwise
     */
    bool IsValidIndex(int x, int y, int z) const;

    /**
     * @brief Returns a copy of the neighbor block in the specified direction.
     *
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @param dir Direction of the neighbor
     *
     * @return Neighbor block at the specified direction
     *
     * @throws std::out_of_range if the neighbor coordinates are invalid
     */
    Block NeighborAt(int x, int y, int z, Direction dir) const;

    /**
     * @brief Returns a reference to the neighbor block in the specified direction.
     *
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @param dir Direction of the neighbor
     *
     * @return Reference to the neighbor block
     *
     * @throws std::out_of_range if the neighbor coordinates are invalid
     */
    Block& NeighborRefAt(int x, int y, int z, Direction dir);

    /**
     * @brief Fills the chunk using a custom filler function.
     *
     * @param filler Function that returns a Block given coordinates (x, y, z)
     */
    void Fill(std::function<Block(int, int, int)> filler);

    /**
     * @brief Returns the total number of blocks in the chunk.
     *
     * @return Total block count
     */
    int BlockCount() const { return Constants::Chunk::size; }

    /**
     * @brief Returns the coordinates of the chunk in world space.
     *
     * @return Chunk coordinates
     */
    glm::ivec2 ChunkCoords() const { return world_coords_; }

    /**
     * @brief Returns a const reference to the internal block array.
     *
     * @return Array of blocks
     */
    const std::array<Block, Constants::Chunk::size>& Data() const { return chunk_; }

private:
    /**
     * @brief Converts 3D block coordinates to a linear array index.
     *
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     *
     * @return Linear index corresponding to (x, y, z)
     */
    int Index(int x, int y, int z) const;
};

#endif // CHUNK_HPP
