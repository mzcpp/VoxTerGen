#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <glad/glad.h>

class VertexArray
{
private:
    GLuint id_;

public:
    VertexArray() = default;

    VertexArray(const VertexArray& other) = delete;
    VertexArray& operator=(const VertexArray& other) = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    ~VertexArray();

    void Initialize();

    void Release();

    void Bind();

    void Unbind();

    void BindVertexBuffer(GLuint binding_index, GLuint vbo_id, GLintptr offset, GLsizei stride);
    
    void BindElementBuffer(GLuint ebo_id);

    void EnableAttribute(GLuint attrib_index);

    void SetAttribute(GLuint attrib_index, GLint size, GLenum type, GLboolean normalized, GLuint relative_offset);

    void SetIntAttribute(GLuint attrib_index, GLint size, GLenum type, GLuint relative_offset);

    void BindAttribute(GLuint attrib_index, GLuint binding_index);

    // Getters
    GLuint Id() const noexcept { return id_; }
};

#endif // VERTEX_ARRAY_HPP

