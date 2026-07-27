#ifndef UNIFORM_BUFFER_HPP
#define UNIFORM_BUFFER_HPP

#include <glad/glad.h>

#include <type_traits>
#include <cassert>

class UniformBuffer
{
private:
	GLuint id_ = 0;
	GLsizeiptr size_ = 0;

public:
	UniformBuffer() = default;

	UniformBuffer(const UniformBuffer& other) = delete;
	UniformBuffer& operator=(const UniformBuffer& other) = delete;

	UniformBuffer(UniformBuffer&& other) noexcept;
	UniformBuffer& operator=(UniformBuffer&& other) noexcept;

	~UniformBuffer();

	void Initialize(GLsizeiptr size, GLuint binding_point);

	void UploadData(GLintptr offset, GLsizeiptr size, const void* data);

	template <typename T>
	void UploadData(const T& object)
	{
		static_assert(std::is_trivially_copyable_v<T>);
		static_assert(!std::is_pointer_v<T>);
    	
		assert(sizeof(T) <= size_);

		UploadData(0, sizeof(T), &object);
	}

private:
	void Release();
};

#endif // UNIFORM_BUFFER_HPP

