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
    std::uint8_t sun_light_;
    std::uint8_t block_light_;

public:
    /**
     * @brief Constructs a block of a given type.
     *
     * @param type Type of the block (default: Air)
     */
    explicit Block(BlockType type = BlockType::Air);

    /**
     * @brief Checks if the block is solid.
     *
     * @return true if the block is solid, false otherwise
     */
    bool IsSolid() const;

    /**
     * @brief Checks if the block is transparent.
     *
     * @return true if the block is transparent (water), false otherwise
     */
    bool IsTransparent() const;
    
    bool IsAir() const;

    bool ShouldRenderFace(const Block& neighbor) const;

    // Getters
    BlockType Type() const;
    std::uint8_t SunLight() const;
    std::uint8_t BlockLight() const;

    // Setters
    void SetType(BlockType type);
    void SetSunLight(std::uint8_t sun_light);
    void SetBlockLight(std::uint8_t block_light);
};

#endif // BLOCK_HPP