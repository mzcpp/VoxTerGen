#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <filesystem>
#include <type_traits>

class ShaderProgram
{
private:
	GLuint id_;
	mutable std::unordered_map<std::string, GLint> uniform_cache_;

public:
	ShaderProgram(const std::filesystem::path& vertex_path,
		const std::filesystem::path& fragment_path, 
		const std::filesystem::path& geometry_path = {});

	ShaderProgram(const std::filesystem::path& compute_path);

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	ShaderProgram(ShaderProgram&& other) noexcept;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;
	
	~ShaderProgram();

	[[nodiscard]] GLuint Id() const noexcept;
	
    void Use() const noexcept;

    template <typename T>
    void Set(std::string_view name, const T& value) const noexcept
    {
        GLint location = GetUniformLocation(name);
        using U = std::remove_cv_t<std::remove_reference_t<T>>;

        // Fundamental types
        if constexpr (std::is_same_v<U, bool>)
        {
            glUniform1i(location, static_cast<GLint>(value));
        }
        else if constexpr (std::is_same_v<U, int>)
        {
            glUniform1i(location, value);
        }
        else if constexpr (std::is_same_v<U, std::uint8_t>)
        {
            glUniform1ui(location, static_cast<GLuint>(value));
        }
        else if constexpr (std::is_same_v<U, unsigned int> || std::is_same_v<U, std::uint32_t>)
        {
            glUniform1ui(location, value);
        }
        else if constexpr (std::is_same_v<U, float>)
        {
            glUniform1f(location, value);
        }

        // Vectors
        else if constexpr (std::is_same_v<U, glm::vec2>)
        {
            glUniform2fv(location, 1, &value[0]);
        }
        else if constexpr (std::is_same_v<U, glm::vec3>)
        {
            glUniform3fv(location, 1, &value[0]);
        }
        else if constexpr (std::is_same_v<U, glm::vec4>)
        {
            glUniform4fv(location, 1, &value[0]);
        }

        // Int vectors
        else if constexpr (std::is_same_v<U, glm::ivec2>)
        {
            glUniform2iv(location, 1, &value[0]);
        }
        else if constexpr (std::is_same_v<U, glm::ivec3>)
        {
            glUniform3iv(location, 1, &value[0]);
        }
        else if constexpr (std::is_same_v<U, glm::ivec4>)
        {
            glUniform4iv(location, 1, &value[0]);
        }

        // Matrices
        else if constexpr (std::is_same_v<U, glm::mat2>)
        {
            glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
        }
        else if constexpr (std::is_same_v<U, glm::mat3>)
        {
            glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
        }
        else if constexpr (std::is_same_v<U, glm::mat4>)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
        }

        // Unsupported type
        else
        {
            static_assert(sizeof(U) == 0, "ShaderProgram::Set() does not support this type.");
        }
    }

private:
	void CheckErrors() const;

	[[nodiscard]] GLint GetUniformLocation(std::string_view name) const noexcept;
};

#endif

