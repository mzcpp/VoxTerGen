#include "core/ResourceManager.hpp"
#include "graphics/Texture2D.hpp"
#include "graphics/ShaderProgram.hpp"
#include "utils/Logger.hpp"
#include "utils/Constants.hpp"

#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>

#include <glad/glad.h>

#include <memory>
#include <filesystem>

void ResourceManager::InitializeResources()
{
    AddTexture("atlas", std::make_unique<TextureUtils::Texture2D>(constants::paths::texture_atlas, true, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST));

    constexpr std::array<int, 6> skybox_z_offsets = { 3, 2, 0, 1, 4, 5 };
    constexpr std::size_t columns_n = 3;
    constexpr std::size_t rows_n = 2;

    //AddTexture("skybox", std::make_unique<TextureUtils::Texture2D>(constants::paths::sky_cubemap, columns_n, rows_n, skybox_z_offsets, true, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR));
    AddShaderProgram("chunk_mesh_shader", std::make_unique<ShaderProgram>(constants::paths::chunk_mesh_vertex_shader, constants::paths::chunk_mesh_fragment_shader));
    AddShaderProgram("block_highlight_shader", std::make_unique<ShaderProgram>(constants::paths::block_highlight_vertex_shader, constants::paths::block_highlight_fragment_shader));
    
    constexpr int font_size = 28;
    LoadFont("default_font", constants::paths::default_font, font_size);
    LoadSound("button_click", constants::paths::button_click);
}

TextureUtils::Texture2D* ResourceManager::GetTexture(const std::string& texture_name) const
{
    const auto texture_it = textures_.find(texture_name);

    if (texture_it == textures_.end())
    {
        Logger::Log(LogLevel::ERROR, "Failed to get a texture! Texture name: {}", texture_name);
        return nullptr;
    }

    return texture_it->second.get();
}
ShaderProgram* ResourceManager::GetShaderProgram(const std::string& shader_program_name) const
{
    const auto shader_it = shader_programs_.find(shader_program_name);

    if (shader_it == shader_programs_.end())
    {
        Logger::Log(LogLevel::ERROR, "Failed to get a shader program! Shader program name: {}", shader_program_name);
        return nullptr;
    }

    return shader_it->second.get();
}

TTF_Font* ResourceManager::GetFont(const std::string& font_name) const
{
    const auto font_it = fonts_.find(font_name);

    if (font_it == fonts_.end())
    {
        Logger::Log(LogLevel::ERROR, "Failed to get a font! Font name: {}", font_name);
        return nullptr;
    }

    return font_it->second.get();
}

Mix_Chunk* ResourceManager::GetSound(const std::string& sound_name) const
{
    const auto sound_it = sounds_.find(sound_name);

    if (sound_it == sounds_.end())
    {
        Logger::Log(LogLevel::ERROR, "Failed to get a sound! Sound name: {}", sound_name);
        return nullptr;
    }

    return sound_it->second.get();
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