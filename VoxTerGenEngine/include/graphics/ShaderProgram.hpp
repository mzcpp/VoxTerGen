#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "graphics/Shader.hpp"

#include <glad/glad.h>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <filesystem>
#include <string_view>
#include <type_traits>

/**
 * @brief Represents an OpenGL shader program.
 *
 * ShaderProgram compiles and links vertex, fragment, geometry, or compute shaders
 * and provides methods to bind the program and set uniform values.
 */
class ShaderProgram
{
private:
    GLuint id_;

public:
    /**
     * @brief Constructs a shader program from vertex, fragment, and optional geometry shaders.
     *
     * @param vertex_path Path to the vertex shader file.
     * @param fragment_path Path to the fragment shader file.
     * @param geometry_path Optional path to the geometry shader file.
     *
     * @throws std::runtime_error if shader compilation or program linking fails.
     */
    ShaderProgram(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path, const std::filesystem::path& geometry_path = {});

    /**
     * @brief Constructs a shader program from a compute shader.
     *
     * @param compute_path Path to the compute shader file.
     *
     * @throws std::runtime_error if shader compilation or program linking fails.
     */
    ShaderProgram(const std::filesystem::path& compute_path);

    ShaderProgram(const ShaderProgram& other) = delete;
    ShaderProgram& operator=(const ShaderProgram& other) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    ~ShaderProgram();

    /**
     * @brief Binds the shader program for use in rendering.
     */
    void Use() const noexcept;

    // Getters
    GLuint Id() const noexcept { return id_; }

    /**
     * @brief Sets a uniform value in the shader program.
     *
     * Supports fundamental types (bool, int, uint, float), glm vectors (vec2, vec3, vec4),
     * integer vectors (ivec2, ivec3, ivec4), and matrices (mat2, mat3, mat4).
     *
     * @tparam T Type of the value to set.
     * @param name Name of the uniform variable in the shader.
     * @param value Value to assign to the uniform.
     *
     * @note Unsupported types will trigger a static_assert at compile time.
     */
    template <typename T>
    void Set(std::string_view name, const T& value) const noexcept
    {
        const GLint location = glGetUniformLocation(id_, name.data());
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
    /**
     * @brief Checks for shader program linking errors.
     *
     * @throws std::runtime_error if program linking failed.
     */
    void CheckErrors() const;

    /**
     * @brief Detaches and deallocates the shader.
     */
    void DetachAndDeleteShader(Shader& shader);
};

#endif // SHADER_PROGRAM_HPP