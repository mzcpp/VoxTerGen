#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "graphics/Texture2D.hpp"
#include "graphics/ShaderProgram.hpp"

#include <unordered_map>
#include <string>

struct TTF_Font;
struct Mix_Chunk;

class ResourceManager
{
private:
    std::unordered_map<std::string, std::unique_ptr<TextureUtils::Texture2D>> textures_;
    std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> shader_programs_;
    std::unordered_map<std::string, TTF_Font*> fonts_;
    std::unordered_map<std::string, Mix_Chunk*> sounds_;

public:
    ResourceManager();

    TextureUtils::Texture2D* GetTexture(const std::string& texture_name);

    ShaderProgram* GetShader(const std::string& shader_name);

    TTF_Font* GetFont(const std::string& font_name);

    Mix_Chunk* GetSound(const std::string& sound_name);
};

#endif // RESOURCE_MANAGER_HPP

