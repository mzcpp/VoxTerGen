#include "core/ResourceManager.hpp"
#include "utils/Logger.hpp"

ResourceManager::ResourceManager();
{

}

Texture2D* ResourceManager::GetTexture(const std::string& texture_name)
{
    const auto texture_it = textures_.find(texture_name);

    if (texture_it == textures_.end())
    {
        Logger::Log(LogLevel::ERROR, "Failed to get a texture! Texture name: {}", texture_name);
        return nullptr;
    }

    return texture_it->second.get();
}

ShaderProgram* ResourceManager::GetShader(const std::string& shader_name)
{
    const auto shader_it = shader_programs_.find(shader_name);

    if (shader_it == shader_programs_.end())
    {
        Logger::Log(LogLevel::ERROR, "Failed to get a shader program! Shader name: {}", shader_name);
        return nullptr;
    }

    return shader_it->second.get();
}