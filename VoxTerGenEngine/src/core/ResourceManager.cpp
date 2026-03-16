#include "core/ResourceManager.hpp"
#include "utils/Logger.hpp"
#include "graphics/Texture2D.hpp"

ResourceManager::ResourceManager()
{

}

TextureUtils::Texture2D* ResourceManager::GetTexture(const std::string & texture_name)
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

TTF_Font* ResourceManager::GetFont(const std::string& font_name)
{
    const auto font_it = fonts_.find(font_name);

    if (font_it == fonts_.end())
    {
        Logger::Log(LogLevel::ERROR, "Failed to get a font! Font name: {}", font_name);
        return nullptr;
    }

    return font_it->second;
}

Mix_Chunk* ResourceManager::GetSound(const std::string& sound_name)
{
    const auto sound_it = sounds_.find(sound_name);

    if (sound_it == sounds_.end())
    {
        Logger::Log(LogLevel::ERROR, "Failed to get a sound! Sound name: {}", sound_name);
        return nullptr;
    }

    return sound_it->second;
}