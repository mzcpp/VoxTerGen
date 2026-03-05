#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "graphics/Texture2D.hpp"
#include "graphics/ShaderProgram.hpp"

#include <unordered_map>
#include <string>

class ResourceManager
{
private:
    std::unordered_map<std::string, std::unique_ptr<Texture2D>> textures_;
    std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> shader_programs_;
    // fonts
    // sounds

public:
    ResourceManager();

    Texture2D* GetTexture(const std::string& texture_name);

    ShaderProgram* GetShader(const std::string& shader_name);
};

#endif // RESOURCE_MANAGER_HPP

