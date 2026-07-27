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

void VertexArray::Initialize()
{
    Release();
    glCreateVertexArrays(1, &id_);
}

void VertexArray::Release()
{
    if (id_ != 0)
	{
		glDeleteVertexArrays(1, &id_);
		id_ = 0;
	}
}

void VertexArray::Bind()
{
    glBindVertexArray(id_);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::BindVertexBuffer(GLuint binding_index, GLuint vbo_id, GLintptr offset, GLsizei stride)
{
    glVertexArrayVertexBuffer(id_, binding_index, vbo_id, offset, stride);
}
    
void VertexArray::BindElementBuffer(GLuint ebo_id)
{
    glVertexArrayElementBuffer(id_, ebo_id);
}

void VertexArray::EnableAttribute(GLuint attrib_index)
{
    glEnableVertexArrayAttrib(id_, attrib_index);
}

void VertexArray::SetAttribute(GLuint attrib_index, GLint size, GLenum type, GLboolean normalized, GLuint relative_offset)
{
    glVertexArrayAttribFormat(id_, attrib_index, size, type, normalized, relative_offset);
}

void VertexArray::SetIntAttribute(GLuint attrib_index, GLint size, GLenum type, GLuint relative_offset)
{
    glVertexArrayAttribIFormat(id_, attrib_index, size, type, relative_offset);
}

void VertexArray::BindAttribute(GLuint attrib_index, GLuint binding_index)
{
    glVertexArrayAttribBinding(id_, attrib_index, binding_index);
}
