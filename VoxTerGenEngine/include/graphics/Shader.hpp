#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <filesystem>
#include <string>

/**
 * @brief Represents an OpenGL shader.
 *
 * The Shader class loads, compiles, and manages an OpenGL shader object
 * from a given source file. It supports move semantics but disallows copying.
 */
class Shader
{
private:
    GLuint id_;
    std::string source_path_;
    GLenum type_;

public:
    /**
     * @brief Constructs and compiles a shader from a file.
     *
     * @param shader_path Path to the shader source file.
     * @param shader_type OpenGL shader type (e.g., GL_VERTEX_SHADER).
     *
     * @throws std::runtime_error if the shader file cannot be opened or compilation fails.
     */
    Shader(const std::filesystem::path& shader_path, GLenum shader_type);
    
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    /**
     * @brief Move constructor.
     *
     * Transfers ownership of the shader object from another instance.
     *
     * @param other Shader instance to move from.
     */
    Shader(Shader&& other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * Transfers ownership of the shader object from another instance.
     *
     * @param other Shader instance to move from.
     * @return Reference to this Shader.
     */
    Shader& operator=(Shader&& other) noexcept;
    
    /**
     * @brief Destructor.
     *
     * Deletes the OpenGL shader object if it has not been released.
     */
    ~Shader();

    /**
     * @brief Returns the OpenGL shader ID.
     *
     * @return Shader object ID.
     */
    GLuint Get() const;

    /**
     * @brief Releases ownership of the shader object.
     *
     * After calling this, the Shader object will no longer manage
     * the OpenGL shader, and the caller is responsible for deletion.
     *
     * @return The released shader ID.
     */
    GLuint Release();

private:
    /**
     * @brief Checks for compilation errors.
     *
     * Queries the OpenGL shader compile status and throws an exception
     * if compilation failed.
     *
     * @throws std::runtime_error if shader compilation failed.
     */
    void CheckErrors();
};

#endif // SHADER_HPP