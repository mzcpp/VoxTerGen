#include "world/Block.hpp"

Block::Block(BlockType type) : type_(type), sun_light_(0), block_light_(0)
{
}

bool Block::IsSolid() const 
{
    return type_ != BlockType::Air && type_ != BlockType::Water;
}

bool Block::IsTransparent() const 
{
    return type_ == BlockType::Water;
}

bool Block::IsAir() const 
{
    return type_ == BlockType::Air;
}

bool Block::ShouldRenderFace(const Block& neighbor) const
{
    if (this->IsAir())
    {
        return false;
    }

    if (neighbor.IsAir())
    {
        return true;
    }

    if (this->Type() == neighbor.Type())
    {
        return false;
    }

    if (!this->IsTransparent() && !neighbor.IsTransparent())
    {
        return false;
    }

    return true;
}

BlockType Block::Type() const 
{
    return type_;
}

void Block::SetType(BlockType type)
{
    type_ = type;
}