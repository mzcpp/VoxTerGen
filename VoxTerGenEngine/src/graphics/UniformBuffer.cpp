#include "graphics/UniformBuffer.hpp"

#include <glad/glad.h>

#include <utility>

UniformBuffer::UniformBuffer() :
	id_(0),
	size_(0)
{
}

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
{
	id_ = std::exchange(other.id_, 0);
	size_ = std::exchange(other.size_, 0);
}

UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept
{
	if (&other == this)
	{
		return *this;
	}

	ReleaseBuffer();

	id_ = std::exchange(other.id_, 0);
	size_ = std::exchange(other.size_, 0);

	return *this;
}

UniformBuffer::~UniformBuffer()
{
	ReleaseBuffer();
}

void UniformBuffer::InitializeBuffer(GLsizeiptr size, GLuint binding_point)
{
	glCreateBuffers(1, &id_);
	size_ = size;
}

void UniformBuffer::ReleaseBuffer()
{
	if (id_ != 0)
	{
		glDeleteBuffers(1, &id_);
	}
}
