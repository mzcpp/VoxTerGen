#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "graphics/Texture2D.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/Shader.hpp"

#include "SDL2_ttf/SDL_ttf.h"
#include "SDL2_mixer/SDL_mixer.h"

#include <unordered_map>
#include <string>
#include <memory>

struct TTF_Font;
struct Mix_Chunk;

class ResourceManager
{
private:
    std::unordered_map<std::string, std::unique_ptr<TextureUtils::Texture2D>> textures_;
    std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> shader_programs_;
    std::unordered_map<std::string, std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>> fonts_;
    std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>> sounds_;
public:
    ResourceManager();

    TextureUtils::Texture2D* GetTexture(const std::string& texture_name);

    ShaderProgram* GetShaderProgram(const std::string& shader_program_name);

    TTF_Font* GetFont(const std::string& font_name);

    Mix_Chunk* GetSound(const std::string& sound_name);
    
    void AddTexture(const std::string& texture_name, std::unique_ptr<TextureUtils::Texture2D> texture);

    void AddShaderProgram(const std::string& shader_program_name, std::unique_ptr<ShaderProgram> shader_program);

    void LoadFont(const std::string& font_name, const std::filesystem::path& path, int font_size);

    void LoadSound(const std::string& sound_name, const std::filesystem::path& path);
};

#endif // RESOURCE_MANAGER_HPP

