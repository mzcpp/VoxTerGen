#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <glad/glad.h>

class Buffer
{
private:
    GLuint id_ = 0;

public:
    Buffer() = default;

    Buffer(const Buffer& other) = delete;
    Buffer& operator=(const Buffer& other) = delete;

    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;

    ~Buffer();

    void Initialize() noexcept;

    void Bind(GLenum target) const noexcept;

    void Unbind(GLenum target) const noexcept;

    void UploadData(GLsizeiptr size, const void* data, GLenum usage) const noexcept;

    void UploadSubData(GLintptr offset, GLsizeiptr size, const void* data) const noexcept;

    // Getters
    GLuint Id() const noexcept { return id_; }

private:
    void Release() noexcept;
};

#endif // BUFFER_HPP

