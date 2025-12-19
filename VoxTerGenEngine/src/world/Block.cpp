#include "world/Block.hpp"


Block::Block(BlockType type) : type_(type)
{

}

bool Block::IsSolid() const
{
    return type_ != BlockType::Air && type_ != BlockType::Water;
}