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

    void Bind();

    void Unbind();
};

#endif // BUFFER_HPP

