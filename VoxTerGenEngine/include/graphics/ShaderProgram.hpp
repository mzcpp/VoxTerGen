#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <filesystem>
#include <type_traits>

/**
 * @brief Represents an OpenGL shader program.
 *
 * ShaderProgram compiles and links vertex, fragment, geometry, or compute shaders
 * and provides methods to bind the program and set uniform values.
 * It supports move semantics but disallows copying.
 */
class ShaderProgram
{
private:
    GLuint id_;
    mutable std::unordered_map<std::string, GLint> uniform_cache_;

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
    ShaderProgram(const std::filesystem::path& vertex_path,
        const std::filesystem::path& fragment_path,
        const std::filesystem::path& geometry_path = {});

    /**
     * @brief Constructs a shader program from a compute shader.
     *
     * @param compute_path Path to the compute shader file.
     *
     * @throws std::runtime_error if shader compilation or program linking fails.
     */
    ShaderProgram(const std::filesystem::path& compute_path);

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    /**
     * @brief Move constructor.
     *
     * Transfers ownership of the shader program from another instance.
     *
     * @param other ShaderProgram instance to move from.
     */
    ShaderProgram(ShaderProgram&& other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * Transfers ownership of the shader program from another instance.
     *
     * @param other ShaderProgram instance to move from.
     * @return Reference to this ShaderProgram.
     */
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    /**
     * @brief Destructor.
     *
     * Deletes the OpenGL program object if it has not been released.
     */
    ~ShaderProgram();

    /**
     * @brief Returns the OpenGL program ID.
     *
     * @return Shader program object ID.
     */
    [[nodiscard]] GLuint Id() const noexcept;

    /**
     * @brief Binds the shader program for use in rendering.
     */
    void Use() const noexcept;

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
        GLint location = GetUniformLocation(name);
        using U = std::remove_cv_t<std::remove_reference_t<T>>;

        // Implementation omitted (template code)
    }

private:
    /**
     * @brief Checks for shader program linking errors.
     *
     * @throws std::runtime_error if program linking failed.
     */
    void CheckErrors() const;

    /**
     * @brief Retrieves the location of a uniform variable.
     *
     * Caches uniform locations to avoid repeated OpenGL queries.
     *
     * @param name Name of the uniform variable.
     * @return Location of the uniform variable, or -1 if not found.
     */
    [[nodiscard]] GLint GetUniformLocation(std::string_view name) const noexcept;
};

#endif // SHADER_PROGRAM_HPP