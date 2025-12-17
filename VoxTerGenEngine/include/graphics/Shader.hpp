#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <filesystem>
#include <string>

class Shader
{
private:
	GLuint id_;
    std::string source_path_;
    GLenum type_;

public:
    Shader(const std::filesystem::path& shader_path, GLenum shader_type);
    
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;
    
    ~Shader();

    GLuint Get() const;

    GLuint Release();

private:
    void CheckErrors();
};

#endif