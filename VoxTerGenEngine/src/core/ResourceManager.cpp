#include "core/ResourceManager.hpp"
#include "utils/Logger.hpp"
#include "graphics/Texture2D.hpp"

ResourceManager::ResourceManager()
{
    AddTexture("atlas", std::make_unique<TextureUtils::Texture2D>(constants::paths::texture_atlas, true, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST));   
    //AddTexture("cubemap", std::make_unique<TextureUtils::Texture2D>(constants::paths::sky_cubemap, true, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR ));
    LoadShader("chunk_mesh_vertex", constants::paths::chunk_mesh_vertex_shader, GL_VERTEX_SHADER);
    LoadShader("chunk_mesh_fragment", constants::paths::chunk_mesh_fragment_shader, GL_FRAGMENT_SHADER);
    LoadFont("default_font", constants::paths::default_font);
    LoadSound("button_click", constants::paths::button_click);
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

void LoadTexture(const std::string& texture_name, std::string_view path)
{
    if (texture_name.empty() || path.empty())
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a texture into resource manager! Texture name: {}, path: {}", texture_name, path);
        return;   
    }

    if (!textures_.try_emplace(texture_name, std::make_unique<TextureUtils::Texture2D>(path).second))
    {
        Logger::Log(LogLevel::INFO, "Texture already exists in resource manager! Texture name: {}, path", texture_name, path);
    }
}

void AddTexture(const std::string& texture_name, std::unique_ptr<TextureUtils::Texture2D> texture)
{
    if (texture_name.empty() || texture == nullptr)
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a texture into resource manager! Texture name: {}, texture: {}", texture_name, texture);
        return;
    }

    if (!textures_.try_emplace(texture_name, std::move(texture).second))
    {
        Logger::Log(LogLevel::INFO, "Texture already exists in resource manager! Texture name: {}", texture_name);
    }
}

void LoadShader(const std::string& shader_name, std::string_view path, GLenum shader_type)
{
    if (shader_name.empty() || path.empty())
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a shader into resource manager! Shader name: {}, path: {}", shader_name, path);
        return;   
    }

    if (!shader_programs_.try_emplace(shader_name, std::make_unique<Shader>(path, shader_type).second))
    {
        Logger::Log(LogLevel::INFO, "Shader already exists in resource manager! Shader name: {}, path", shader_name, path);
    }
}

void AddShader(const std::string& shader_name, std::unique_ptr<Shader> shader)
{
    if (shader_name.empty() || shader == nullptr)
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a shader into resource manager! Shader name: {}, shader: {}", shader_name, shader);
        return;   
    }

    if (!shader_programs_.try_emplace(shader_name, std::move(shader).second))
    {
        Logger::Log(LogLevel::INFO, "Shader already exists in resource manager! Shader name: {}", shader_name);
    }
}

void LoadFont(const std::string& font_name, std::string_view path, int font_size)
{
    if (font_name.empty() || path.empty())
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a font into resource manager! Font name: {}, path: {}", font_name, path);
        return;   
    }

    if (!fonts_.try_emplace(font_name, { TTF_OpenFont(path.data(), font_size), TTF_CloseFont }))
    {
        Logger::Log(LogLevel::INFO, "Font already exists in resource manager! Font name: {}, path", font_name, path);
    }
}

void LoadSound(const std::string& sound_name, std::string_view path)
{
    if (sound_name.empty() || path.empty())
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a sound into resource manager! Sound name: {}, path: {}", sound_name, path);
        return;   
    }

    if (!sounds_.try_emplace(sound_name, { Mix_LoadWAV(path.data()), Mix_FreeChunk }))
    {
        Logger::Log(LogLevel::INFO, "Sound already exists in resource manager! Sound name: {}, path", sound_name, path);
    }
}