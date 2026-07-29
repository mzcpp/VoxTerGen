#include "graphics/VertexArray.hpp"

#include <glad/glad.h>

#include <utility>

VertexArray::VertexArray(VertexArray&& other) noexcept
{
	id_ = std::exchange(other.id_, 0);
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (&other == this)
	{
		return *this;
	}

	Release();

	id_ = std::exchange(other.id_, 0);

	return *this;
}

VertexArray::~VertexArray()
{
    Release();
}

void VertexArray::Initialize() noexcept
{
    Release();
    glCreateVertexArrays(1, &id_);
}

void VertexArray::Bind() const noexcept
{
    glBindVertexArray(id_);
}

void VertexArray::Unbind() const noexcept
{
    glBindVertexArray(0);
}

void VertexArray::BindVertexBuffer(GLuint binding_index, const Buffer& vertex_buffer, GLintptr offset, GLsizei stride) const noexcept
{
    glVertexArrayVertexBuffer(id_, binding_index, vertex_buffer.Id(), offset, stride);
}
    
void VertexArray::BindElementBuffer(const Buffer& element_buffer) const noexcept
{
    glVertexArrayElementBuffer(id_, element_buffer.Id());
}

void VertexArray::EnableAttribute(GLuint attrib_index) const noexcept
{
    glEnableVertexArrayAttrib(id_, attrib_index);
}

void VertexArray::SetAttribute(GLuint attrib_index, GLint size, GLenum type, GLboolean normalized, GLuint relative_offset) const noexcept
{
    glVertexArrayAttribFormat(id_, attrib_index, size, type, normalized, relative_offset);
}

void VertexArray::SetIntAttribute(GLuint attrib_index, GLint size, GLenum type, GLuint relative_offset) const noexcept
{
    glVertexArrayAttribIFormat(id_, attrib_index, size, type, relative_offset);
}

void VertexArray::BindAttribute(GLuint attrib_index, GLuint binding_index) const noexcept
{
    glVertexArrayAttribBinding(id_, attrib_index, binding_index);
}

void VertexArray::Release() noexcept
{
    if (id_ != 0)
	{
		glDeleteVertexArrays(1, &id_);
		id_ = 0;
	}
}