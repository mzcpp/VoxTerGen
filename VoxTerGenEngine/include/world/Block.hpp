#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstdint>

enum class BlockType : uint8_t 
{
    Air = 0,
    Dirt,
    Stone,
    Water,
    Sand,
    Snow
};

class Block
{
private:
    BlockType type_;

public:
	Block(BlockType type = BlockType::Air);

    bool IsSolid() const;
};

#endif

