#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstdint>
#include <unordered_map>

#include <glm/vec3.hpp>

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
    Bedrock, 
    Invalid
};

class Block;

struct BlockInfo
{
    Block block_;
    glm::ivec3 absolute_pos_;
};

template <typename Fnc>
concept BlockQuery = std::invocable<Fnc, glm::ivec3>&& std::convertible_to<std::invoke_result_t<Fnc, glm::ivec3>, BlockInfo>;

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