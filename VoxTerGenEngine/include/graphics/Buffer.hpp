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

    void Initialize();

    void Release();

    void Bind(GLenum target);

    void Unbind(GLenum target);

    void UploadData(GLsizeiptr size, const void* data, GLenum usage);

    void UploadSubData(GLintptr offset, GLsizeiptr size, const void* data);

    // Getters
    GLuint Id() const noexcept { return id_; }
};

#endif // BUFFER_HPP

