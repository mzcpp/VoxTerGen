#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <glad/glad.h>

class Buffer
{
private:
    GLuint id_;

public:
    Buffer() = default;

    Buffer(const Buffer& other) = delete;
    Buffer& operator=(const Buffer& other) = delete;

    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;

    ~Buffer();

    void Initialize();

    void Release();

    void Bind(GLuint buffer);

    void Unbind();

    void UploadData(GLsizeiptr size, const void* data, GLenum usage);
};

#endif // BUFFER_HPP

