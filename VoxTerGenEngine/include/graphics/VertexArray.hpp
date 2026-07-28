#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "graphics/Buffer.hpp"

#include <glad/glad.h>

class VertexArray
{
private:
    GLuint id_ = 0;

public:
    VertexArray() = default;

    VertexArray(const VertexArray& other) = delete;
    VertexArray& operator=(const VertexArray& other) = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    ~VertexArray();

    void Initialize() noexcept;

    void Bind() const noexcept;

    void Unbind() const noexcept;

    void BindVertexBuffer(GLuint binding_index, const Buffer& vertex_buffer, GLintptr offset, GLsizei stride) const noexcept;
    
    void BindElementBuffer(const Buffer& element_buffer) const noexcept;

    void EnableAttribute(GLuint attrib_index) const noexcept;

    void SetAttribute(GLuint attrib_index, GLint size, GLenum type, GLboolean normalized, GLuint relative_offset) const noexcept;

    void SetIntAttribute(GLuint attrib_index, GLint size, GLenum type, GLuint relative_offset) const noexcept;

    void BindAttribute(GLuint attrib_index, GLuint binding_index) const noexcept;

    // Getters
    GLuint Id() const noexcept { return id_; }

private:
    void Release() noexcept;
};

#endif // VERTEX_ARRAY_HPP

