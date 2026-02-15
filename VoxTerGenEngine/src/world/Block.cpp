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

std::uint8_t Block::SunLight() const
{
    return sun_light_;
}

std::uint8_t Block::BlockLight() const
{
    return block_light_;
}

void Block::SetType(BlockType type)
{
    type_ = type;
}

void Block::SetSunLight(std::uint8_t sun_light)
{
    sun_light_ = sun_light;
}

void Block::SetBlockLight(std::uint8_t block_light)
{
    block_light_ = block_light;
}