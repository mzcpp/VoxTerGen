#include "graphics/Buffer.hpp"


Buffer::Buffer(Buffer&& other) noexcept
{

}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{

}

Buffer::~Buffer()
{
    Release();
}

void Buffer::Initialize()
{
    Release();
    glCreateBuffers(1, &id_);
}

void Buffer::Release()
{
    if (id_ != 0)
	{
		glDeleteBuffers(1, &id_);
		id_ = 0;
	}
}

void Buffer::Bind()
{
    
}

void Buffer::Unbind()
{
    
}