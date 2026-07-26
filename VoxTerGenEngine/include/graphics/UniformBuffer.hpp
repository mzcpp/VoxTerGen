#ifndef UNIFORM_BUFFER_HPP
#define UNIFORM_BUFFER_HPP

#include <glad/glad.h>

class UniformBuffer
{
private:
	GLuint id_;
	GLsizeiptr size_;

public:
	UniformBuffer();

	UniformBuffer(const UniformBuffer& other) = delete;
	UniformBuffer& operator=(const UniformBuffer& other) = delete;

	UniformBuffer(UniformBuffer&& other) noexcept;
	UniformBuffer& operator=(UniformBuffer&& other) noexcept;

	~UniformBuffer();

	void InitializeBuffer(GLsizeiptr size, GLuint binding_point);

	void ReleaseBuffer();
};

#endif // UNIFORM_BUFFER_HPP

