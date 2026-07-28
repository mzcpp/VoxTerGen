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

void Buffer::Bind(GLenum target)
{
	glBindBuffer(target, id_);
}

void Buffer::Unbind(GLenum target)
{
	glBindBuffer(target, 0);
}

void Buffer::UploadData(GLsizeiptr size, const void* data, GLenum usage)
{
	glNamedBufferData(id_, size, data, usage);
}

void Buffer::UploadSubData(GLintptr offset, GLsizeiptr size, const void* data)
{
    glNamedBufferSubData(id_, offset, size, data);
}