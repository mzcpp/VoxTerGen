#include "graphics/UniformBuffer.hpp"

#include <glad/glad.h>

#include <utility>
#include <cassert>

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

	Release();

	id_ = std::exchange(other.id_, 0);
	size_ = std::exchange(other.size_, 0);

	return *this;
}

UniformBuffer::~UniformBuffer()
{
	Release();
}

void UniformBuffer::Initialize(GLsizeiptr size, GLuint binding_point) noexcept
{
	Release();
	glCreateBuffers(1, &id_);
	size_ = size;

	glNamedBufferData(id_, size_, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, id_);
}

void UniformBuffer::UploadSubData(GLintptr offset, GLsizeiptr size, const void* data) const noexcept
{
	assert(offset >= 0);
	assert(size >= 0);
	assert(offset + size <= size_);
	glNamedBufferSubData(id_, offset, size, data);
}

void UniformBuffer::Release() noexcept
{
	if (id_ != 0)
	{
		glDeleteBuffers(1, &id_);
		id_ = 0;
		size_ = 0;
	}
}
