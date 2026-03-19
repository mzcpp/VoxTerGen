#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "graphics/Texture2D.hpp"
#include "graphics/ShaderProgram.hpp"

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
    std::unordered_map<std::string, std::unique_ptr<TTF_Font*, decltype(&TTF_CloseFont)>> fonts_;
    std::unordered_map<std::string, std::unique_ptr<TTF_Font*, decltype(&Mix_FreeChunk)>> sounds_;
public:
    ResourceManager();

    TextureUtils::Texture2D* GetTexture(const std::string& texture_name);

    ShaderProgram* GetShader(const std::string& shader_name);

    TTF_Font* GetFont(const std::string& font_name);

    Mix_Chunk* GetSound(const std::string& sound_name);

    void LoadTexture(const std::string& texture_name, std::string_view path)
    
    void AddTexture(const std::string& texture_name, std::unique_ptr<TextureUtils::Texture2D> texture);
    
    void LoadShader(const std::string& shader_name, std::string_view path);

    void AddShader(const std::string& shader_name, std::unique_ptr<Shader> shader);

    void LoadFont(const std::string& font_name, std::string_view path, int font_size);

    void LoadSound(const std::string& sound_name, std::string_view path);
};

#endif // RESOURCE_MANAGER_HPP

