#include "graphics/ShaderProgram.hpp"
#include "graphics/Shader.hpp"

#include <glm/glm.hpp>

#include <filesystem>
#include <stdexcept>
#include <vector>
#include <string>
#include <optional>

ShaderProgram::ShaderProgram(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path, const std::filesystem::path& geometry_path) 
    : id_(0)
{
    Shader vertex_shader(vertex_path, GL_VERTEX_SHADER);
    Shader fragment_shader(fragment_path, GL_FRAGMENT_SHADER);
    std::optional<Shader> geometry_shader;

    if (!geometry_path.empty())
    {
        geometry_shader.emplace(geometry_path, GL_GEOMETRY_SHADER);
    }

    id_ = glCreateProgram();

    if (id_ == 0)
    {
        throw std::runtime_error("Failed to create shader program");
    }

    glAttachShader(id_, vertex_shader.Id());
    glAttachShader(id_, fragment_shader.Id());

    if (geometry_shader.has_value())
    {
        glAttachShader(id_, geometry_shader->Id());
    }
    
    glLinkProgram(id_);

    try
    {
        CheckErrors();
    }
    catch (...)
    {
        DetachAndDeleteShader(vertex_shader);
        DetachAndDeleteShader(fragment_shader);

        if (geometry_shader.has_value())
        {
            DetachAndDeleteShader(*geometry_shader);
        }

        glDeleteProgram(id_);
        id_ = 0;
        throw;
    }

    DetachAndDeleteShader(vertex_shader);
    DetachAndDeleteShader(fragment_shader);

    if (geometry_shader.has_value())
    {
        DetachAndDeleteShader(*geometry_shader);
    }
}

ShaderProgram::ShaderProgram(const std::filesystem::path& compute_path) : id_(0)
{
    Shader compute_shader(compute_path, GL_COMPUTE_SHADER);
    
    id_ = glCreateProgram();

    if (id_ == 0)
    {
        throw std::runtime_error("Failed to create shader program");
    }

    glAttachShader(id_, compute_shader.Id());
    glLinkProgram(id_);
    
    try
    {
        CheckErrors();
    }
    catch (...)
    {
        DetachAndDeleteShader(compute_shader);
        glDeleteProgram(id_);
        id_ = 0;
        throw;
    }

    DetachAndDeleteShader(compute_shader);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
{
    if (id_ != 0)
    {
        glDeleteProgram(id_);
    }

    id_ = std::exchange(other.id_, 0);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    if (id_ != 0)
    {
        glDeleteProgram(id_);
    }

    id_ = std::exchange(other.id_, 0);

    return *this;
}

ShaderProgram::~ShaderProgram()
{
    if (id_ != 0)
    {
        glDeleteProgram(id_);
    }
}

void ShaderProgram::Use() const noexcept
{
	glUseProgram(id_);
}

void ShaderProgram::CheckErrors() const
{
    GLint success = GL_TRUE;

    glGetProgramiv(id_, GL_LINK_STATUS, &success);

    if (success == GL_TRUE)
    {
        return;
    }

    GLint length = 0;

    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> info_log(length ? length : 1);
    GLsizei actual_length = 0;
    glGetProgramInfoLog(id_, length, &actual_length, info_log.data());
    throw std::runtime_error(std::string("Program linking failed:\n") + std::string(info_log.data(), actual_length));
}

void ShaderProgram::DetachAndDeleteShader(Shader& shader)
{
    glDetachShader(id_, shader.Id());
    glDeleteShader(shader.Release());
}