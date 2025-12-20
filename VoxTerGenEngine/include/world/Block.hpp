#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstdint>

/**
 * @brief Enumeration of all possible block types in the world.
 */
enum class BlockType : uint8_t
{
    Air = 0,
    Dirt, 
    Stone, 
    Water, 
    Sand, 
    Snow, 
    Invalid
};

/**
 * @brief Represents a block in the world.
 *
 * Encapsulates the block type and provides utility methods
 * to query properties like solidity and transparency.
 */
class Block final
{
private:
    BlockType type_;

public:
    /**
     * @brief Constructs a block of a given type.
     *
     * @param type Type of the block (default: Air)
     */
    explicit Block(BlockType type = BlockType::Air) : type_(type)
    {
    }

    /**
     * @brief Checks if the block is solid.
     *
     * @return true if the block is solid, false otherwise
     */
    bool IsSolid() const { return type_ != BlockType::Air && type_ != BlockType::Water; }

    /**
     * @brief Checks if the block is transparent.
     *
     * @return true if the block is transparent (water), false otherwise
     */
    bool IsTransparent() const { return type_ == BlockType::Water; }

    /**
     * @brief Returns the block type.
     *
     * @return The BlockType of this block
     */
    BlockType Type() const { return type_; }
};

#endif // BLOCK_HPP