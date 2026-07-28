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

	void Initialize(GLsizeiptr size, GLuint binding_point) noexcept;

	void UploadSubData(GLintptr offset, GLsizeiptr size, const void* data) const noexcept;

	template <typename T>
	void UploadData(const T& object) const noexcept
	{
		static_assert(std::is_trivially_copyable_v<T>);
		static_assert(!std::is_pointer_v<T>);
    	
		assert(sizeof(T) <= size_);

		UploadSubData(0, sizeof(T), &object);
	}

private:
	void Release() noexcept;
};

#endif // UNIFORM_BUFFER_HPP

