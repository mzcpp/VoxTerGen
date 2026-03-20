#include "core/ResourceManager.hpp"
#include "graphics/Texture2D.hpp"
#include "utils/Logger.hpp"
#include "utils/Constants.hpp"

#include "SDL2_ttf/SDL_ttf.h"
#include "SDL2_mixer/SDL_mixer.h"

#include <memory>
#include <filesystem>

ResourceManager::ResourceManager()
{
    //AddTexture("atlas", std::make_unique<TextureUtils::Texture2D>(constants::paths::texture_atlas, true, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST));   
    ////AddTexture("cubemap", std::make_unique<TextureUtils::Texture2D>(constants::paths::sky_cubemap, true, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR ));
    //LoadShader("chunk_mesh_vertex", constants::paths::chunk_mesh_vertex_shader, GL_VERTEX_SHADER);
    //LoadShader("chunk_mesh_fragment", constants::paths::chunk_mesh_fragment_shader, GL_FRAGMENT_SHADER);
    //
    //constexpr int font_size = 28;
    //LoadFont("default_font", constants::paths::default_font, font_size);
    //
    //LoadSound("button_click", constants::paths::button_click);
}

TextureUtils::Texture2D* ResourceManager::GetTexture(const std::string& texture_name)
{
    const auto texture_it = textures_.find(texture_name);

    if (texture_it == textures_.end())
    {
        Logger::Log(LogLevel::ERROR, "Failed to get a texture! Texture name: {}", texture_name);
        return nullptr;
    }

    return texture_it->second.get();
}
ShaderProgram* ResourceManager::GetShaderProgram(const std::string& shader_program_name)
{
    const auto shader_it = shader_programs_.find(shader_program_name);

    if (shader_it == shader_programs_.end())
    {
        Logger::Log(LogLevel::ERROR, "Failed to get a shader program! Shader program name: {}", shader_program_name);
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

    return font_it->second.get();
}

Mix_Chunk* ResourceManager::GetSound(const std::string& sound_name)
{
    const auto sound_it = sounds_.find(sound_name);

    if (sound_it == sounds_.end())
    {
        Logger::Log(LogLevel::ERROR, "Failed to get a sound! Sound name: {}", sound_name);
        return nullptr;
    }

    return sound_it->second.get();
}

void ResourceManager::LoadTexture(const std::string& texture_name, const std::filesystem::path& path)
{
    if (texture_name.empty() || path.empty())
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a texture into resource manager! Texture name: {}, path: {}", texture_name, path.string());
        return;   
    }

    if (!textures_.try_emplace(texture_name, std::make_unique<TextureUtils::Texture2D>(path.string())).second)
    {
        Logger::Log(LogLevel::INFO, "Texture already exists in resource manager! Texture name: {}, path", texture_name, path.string());
    }
}

void ResourceManager::AddTexture(const std::string& texture_name, std::unique_ptr<TextureUtils::Texture2D> texture)
{
    if (texture_name.empty() || texture == nullptr)
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a texture into resource manager! Texture name: {}", texture_name);
        return;
    }

    if (!textures_.try_emplace(texture_name, std::move(texture)).second)
    {
        Logger::Log(LogLevel::INFO, "Texture already exists in resource manager! Texture name: {}", texture_name);
    }
}

void ResourceManager::LoadShaderProgram(const std::string& shader_program_name, std::span<const std::filesystem::path> paths)
{
    if (shader_program_name.empty() || paths.empty() || paths.size() > 3)
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a shader into resource manager! Shader name: {}, paths size: {}", shader_program_name, paths.size());
        return;
    }

    if (shader_programs_.contains(shader_program_name))
    {
        Logger::Log(LogLevel::INFO, "Shader program already exists in resource manager! Shader program name: {}", shader_program_name);
        return;
    }

    switch (paths.size())
    {
    case 1:
        shader_programs_.emplace(shader_program_name, std::make_unique<ShaderProgram>(paths[0]));
        break;

    case 2:
        shader_programs_.emplace(shader_program_name, std::make_unique<ShaderProgram>(paths[0], paths[1]));
        break;

    case 3:
        shader_programs_.emplace(shader_program_name, std::make_unique<ShaderProgram>(paths[0], paths[1], paths[2]));
        break;
    }
}

void ResourceManager::AddShaderProgram(const std::string& shader_program_name, std::unique_ptr<ShaderProgram> shader_program)
{
    if (shader_program_name.empty() || shader_program == nullptr)
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a shader program into resource manager! Shader program name: {}", shader_program_name);
        return;   
    }

    if (!shader_programs_.try_emplace(shader_program_name, std::move(shader_program)).second)
    {
        Logger::Log(LogLevel::INFO, "Shader program already exists in resource manager! Shader program name: {}", shader_program_name);
    }
}

void ResourceManager::LoadFont(const std::string& font_name, const std::filesystem::path& path, int font_size)
{
    if (font_name.empty() || path.empty())
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a font into resource manager! Font name: {}, path: {}", font_name, path.string());
        return;   
    }

    const std::string path_str = path.string();
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font_ptr(TTF_OpenFont(path_str.c_str(), font_size), TTF_CloseFont);

    if (!fonts_.try_emplace(font_name, std::move(font_ptr)).second)
    {
        Logger::Log(LogLevel::INFO, "Font already exists in resource manager! Font name: {}, path", font_name, path_str);
    }
}

void ResourceManager::LoadSound(const std::string& sound_name, const std::filesystem::path& path)
{
    if (sound_name.empty() || path.empty())
    {
        Logger::Log(LogLevel::ERROR, "Failed to load a sound into resource manager! Sound name: {}, path: {}", sound_name, path.string());
        return;   
    }

    const std::string path_str = path.string();
    std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> sound_ptr(Mix_LoadWAV(path_str.c_str()), Mix_FreeChunk);

    if (!sounds_.try_emplace(sound_name, std::move(sound_ptr)).second)
    {
        Logger::Log(LogLevel::INFO, "Sound already exists in resource manager! Sound name: {}, path", sound_name, path_str);
    }
}