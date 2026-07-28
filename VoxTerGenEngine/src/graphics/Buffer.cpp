#include "graphics/Buffer.hpp"

#include <glad/glad.h>

#include <utility>

Buffer::Buffer(Buffer&& other) noexcept
{
    id_ = std::exchange(other.id_, 0);
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
	if (&other == this)
	{
		return *this;
	}

	Release();

	id_ = std::exchange(other.id_, 0);

	return *this;
}

Buffer::~Buffer()
{
    Release();
}

void Buffer::Initialize() noexcept
{
    Release();
    glCreateBuffers(1, &id_);
}

void Buffer::Bind(GLenum target) const noexcept
{
	glBindBuffer(target, id_);
}

void Buffer::Unbind(GLenum target) const noexcept
{
	glBindBuffer(target, 0);
}

void Buffer::UploadData(GLsizeiptr size, const void* data, GLenum usage) const noexcept
{
	glNamedBufferData(id_, size, data, usage);
}

void Buffer::UploadSubData(GLintptr offset, GLsizeiptr size, const void* data) const noexcept
{
    glNamedBufferSubData(id_, offset, size, data);
}

void Buffer::Release() noexcept
{
    if (id_ != 0)
	{
		glDeleteBuffers(1, &id_);
		id_ = 0;
	}
}