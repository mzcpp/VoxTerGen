#include "graphics/Shader.hpp"

#include <stdexcept>
#include <vector>
#include <fstream>

Shader::Shader(const std::filesystem::path& shader_path, GLenum shader_type) : 
    id_(0), source_path_(shader_path.string()), type_(shader_type)
{
    std::ifstream file(source_path_);

    if (!file)
    {
        throw std::runtime_error("Failed to open shader file: " + source_path_);
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string shader_string = ss.str();
    const char* shader_code = shader_string.c_str();

    id_ = glCreateShader(type_);
    glShaderSource(id_, 1, &shader_code, NULL);
    glCompileShader(id_);

    CheckErrors();
}

Shader::Shader(Shader&& other) noexcept
{
    id_ = std::exchange(other.id_, 0);
    source_path_ = std::exchange(other.source_path_, "");
    type_ = std::exchange(other.type_, 0);
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        if (id_ != 0)
        {
            glDeleteShader(id_);
        }

        id_ = std::exchange(other.id_, 0);
        source_path_ = std::exchange(other.source_path_, "");
        type_ = std::exchange(other.type_, 0);
    }
    return *this;
}

Shader::~Shader()
{
    if (id_ != 0)
    {
        glDeleteShader(id_);
    }
}

GLuint Shader::Get() const
{
    return id_;
}

GLuint Shader::Release()
{
    GLuint tmp = id_;
    id_ = 0;
    return tmp;
}

void Shader::CheckErrors()
{
    GLint success = GL_TRUE;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);

    if (success == GL_TRUE)
    {
        return;
    }

    GLint length = 0;
    glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> info_log(length ? length : 1);
    GLsizei actual_length = 0;
    
    glGetShaderInfoLog(id_, length, &actual_length, info_log.data());
    
    throw std::runtime_error("Shader compilation failed (" + source_path_ + "):\n" + std::string(info_log.data(), actual_length));
}
