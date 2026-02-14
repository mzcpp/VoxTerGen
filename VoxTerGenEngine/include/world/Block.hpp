#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstdint>

/**
 * @brief Enumeration of all possible block types in the world.
 */
enum class BlockType : std::uint8_t
{
    Air = 0, 
    Grass, 
    Dirt, 
    Stone, 
    Water, 
    Sand, 
    Snow, 
    Invalid
};

/**
 * @brief Represents a block in the world.
 */
class Block
{
private:
    BlockType type_;
    std::uint32_t sun_light_;
    std::uint32_t block_light_;

public:
    /**
     * @brief Constructs a block of a given type.
     *
     * @param type Type of the block (default: Air)
     */
    explicit Block(BlockType type = BlockType::Air) : type_(type), sun_light_(0), block_light_(0)
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

    // Getters
    BlockType Type() const { return type_; }

    // Setters
    void SetType(BlockType type) { type_ = type; }
};

#endif // BLOCK_HPP