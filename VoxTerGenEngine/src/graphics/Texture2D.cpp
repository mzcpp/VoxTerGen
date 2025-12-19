#include "graphics/Texture2D.hpp"
#include "utils/Logger.hpp"

#include "stb_image.h"

#include <glad/glad/glad.h>

#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <cmath>

namespace TextureUtils
{
    static void DetermineFormats(int n_components, bool sRGB, GLenum* internal_format, GLenum* data_format)
    {
        switch (n_components)
        {
            case 1:
                *internal_format = GL_R8;
                *data_format = GL_RED;
                break;
            case 3:
                *internal_format = sRGB ? GL_SRGB8 : GL_RGB8;
                *data_format = GL_RGB;
                break;
            case 4:
                *internal_format = sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
                *data_format = GL_RGBA;
                break;
            default:
                throw std::runtime_error("Unsupported texture channel count");
        }
    }

    Texture2D::Texture2D(std::string_view path, bool sRGB, bool generate_mipmaps,
        GLenum wrap_s, GLenum wrap_t, GLenum min_filter, GLenum mag_filter) 
        : 
        type_(TextureType::Texture2D), 
        target_(GL_TEXTURE_2D)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &texture_id_);
        int n_components = 0;
        
        std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> data(stbi_load(path.data(), &width_, &height_, &n_components, 0), stbi_image_free);

        if (!data.get())
        {
            glDeleteTextures(1, &texture_id_);
            Logger::Log(LogLevel::INFO, "Failed to open texture file: {}", path.data());
            throw std::runtime_error("Failed to open texture file: " + std::string(path) + " Error: " + stbi_failure_reason());
        }

        if (width_ <= 0 || height_ <= 0)
        {
            glDeleteTextures(1, &texture_id_);
            Logger::Log(LogLevel::INFO, "Invalid dimensions for texture '{}': width {}, height {}", path, width_, height_);
            throw std::runtime_error("Invalid dimensions of texture");
        }

        DetermineFormats(n_components, sRGB, &internal_format_, &data_format_);

        const int levels = generate_mipmaps ? 1 + static_cast<int>(std::floor(std::log2(std::max(width_, height_)))) : 1;
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTextureStorage2D(texture_id_, levels, internal_format_, width_, height_);
        glTextureSubImage2D(texture_id_, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data.get());

        if (generate_mipmaps)
        {
            glGenerateTextureMipmap(texture_id_);
        }

        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_S, data_format_ == GL_RGBA ? DEFAULT_WRAP_RGBA : wrap_s);
        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_T, data_format_ == GL_RGBA ? DEFAULT_WRAP_RGBA : wrap_t);
        glTextureParameteri(texture_id_, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(texture_id_, GL_TEXTURE_MAG_FILTER, mag_filter);
    }

    Texture2D::Texture2D(int width, int height, const unsigned char* data,
        GLenum format, bool generate_mipmaps, GLenum wrap_s, GLenum wrap_t, GLenum min_filter, GLenum mag_filter)
        : 
        width_(width), 
        height_(height), 
        internal_format_(format), 
        data_format_(format), 
        type_(TextureType::Texture2D), 
        target_(GL_TEXTURE_2D)
    {
        if (width_ <= 0 || height_ <= 0)
        {
            Logger::Log(LogLevel::INFO, "Invalid dimensions for texture: width {}, height {}", width_, height_);
            throw std::runtime_error("Invalid dimensions of texture");
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &texture_id_);
        
        const int levels = generate_mipmaps ? 1 + static_cast<int>(std::floor(std::log2(std::max(width_, height_)))) : 1;
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTextureStorage2D(texture_id_, levels, internal_format_, width_, height_);
        glTextureSubImage2D(texture_id_, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data);

        if (generate_mipmaps)
        {
            glGenerateTextureMipmap(texture_id_);
        }

        glTextureParameteri(texture_id_, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(texture_id_, GL_TEXTURE_MAG_FILTER, mag_filter);
        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_S, wrap_s);
        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_T, wrap_t);
    }

    Texture2D::Texture2D(std::span<std::string_view> paths, bool sRGB, bool generate_mipmaps,
        GLenum wrap_s, GLenum wrap_t, GLenum wrap_r, GLenum min_filter, GLenum mag_filter)
        : type_(TextureType::CubeMap), target_(GL_TEXTURE_CUBE_MAP)
    {
        if (paths.size() != 6)
        {
            glDeleteTextures(1, &texture_id_);
            Logger::Log(LogLevel::INFO, "Cubemap requires 6 faces! Currently: {}", paths.size());
            throw std::runtime_error("Cubemap requires 6 faces!");
        }

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &texture_id_);

        int n_components = 0;
        int w = 0;
        int h = 0;

        {
            std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> first_img(stbi_load(paths[0].data(), &w, &h, &n_components, 0), stbi_image_free);

            if (!first_img)
            {
                glDeleteTextures(1, &texture_id_);
                Logger::Log(LogLevel::INFO, "Failed to load cubemap face: {}", paths[0]);
                throw std::runtime_error("Failed to load cubemap face: " + std::string(paths[0]));
            }

            if (w <= 0 || h <= 0)
            {
                glDeleteTextures(1, &texture_id_);
                Logger::Log(LogLevel::INFO, "Invalid dimensions for texture '{}': width {}, height {}", paths[0], width_, height_);
                throw std::runtime_error("Invalid dimensions of texture");
            }

            DetermineFormats(n_components, sRGB, &internal_format_, &data_format_);

            width_ = w;
            height_ = h;
        }

        const int levels = generate_mipmaps ? 1 + static_cast<int>(std::floor(std::log2(std::max(width_, height_)))) : 1;
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTextureStorage3D(texture_id_, levels, internal_format_, width_, height_, 6);

        for (std::size_t i = 0; i < paths.size(); i++)
        {
            int tw = 0;
            int th = 0;
            int comp = 0;

            std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> img(stbi_load(paths[i].data(), &tw, &th, &comp, 0), stbi_image_free);

            if (!img)
            {
                glDeleteTextures(1, &texture_id_);
                Logger::Log(LogLevel::INFO, "Failed to load cubemap face: {}", paths[i].data());
                throw std::runtime_error("Failed to load cubemap face: " + std::string(paths[i]));
            }

            if (tw != width_ || th != height_)
            {
                glDeleteTextures(1, &texture_id_);
                Logger::Log(LogLevel::INFO, "Cubemap faces have mismatched sizes! '{}': width {}, height {}", paths[i], width_, height_);
                throw std::runtime_error("Cubemap faces have mismatched sizes");
            }

            if (comp != n_components)
            {
                glDeleteTextures(1, &texture_id_);
                Logger::Log(LogLevel::INFO, "Cubemap faces have mismatched sizes! '{}': width {}, height {}", paths[i], width_, height_);
                throw std::runtime_error("Cubemap faces have mismatched channel counts");
            }

            glTextureSubImage3D(texture_id_, 0, 0, 0, static_cast<GLint>(i), width_, height_, 1, data_format_, GL_UNSIGNED_BYTE, img.get());
        }
        
        if (generate_mipmaps)
        {
            glGenerateTextureMipmap(texture_id_);
        }
        
        glTextureParameteri(texture_id_, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(texture_id_, GL_TEXTURE_MAG_FILTER, mag_filter);
        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_S, wrap_s);
        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_T, wrap_t);
        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_R, wrap_r);
    }

    Texture2D::Texture2D(Texture2D&& other) noexcept
    {
        texture_id_ = std::exchange(other.texture_id_, 0);
        width_ = std::exchange(other.width_, 0);
        height_ = std::exchange(other.height_, 0);
        internal_format_ = std::exchange(other.internal_format_, 0);
        data_format_ = std::exchange(other.data_format_, 0);
        type_ = std::exchange(other.type_, TextureType::Texture2D);
        target_ = std::exchange(other.target_, GL_TEXTURE_2D);
    }

    Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        if (texture_id_ != 0)
        {
            glDeleteTextures(1, &texture_id_);
        }

        texture_id_ = std::exchange(other.texture_id_, 0);
        width_ = std::exchange(other.width_, 0);
        height_ = std::exchange(other.height_, 0);
        internal_format_ = std::exchange(other.internal_format_, 0);
        data_format_ = std::exchange(other.data_format_, 0);
        type_ = std::exchange(other.type_, TextureType::Texture2D);
        target_ = std::exchange(other.target_, GL_TEXTURE_2D);

        return *this;
    }

    Texture2D::~Texture2D()
    {
        if (texture_id_ != 0)
        {
            glDeleteTextures(1, &texture_id_);
        }
    }

    void Texture2D::Bind(GLuint unit_index) const noexcept
    {
        glBindTextureUnit(unit_index, texture_id_);
    }

    void Texture2D::Unbind(GLuint unit_index) const noexcept
    {
        glBindTextureUnit(unit_index, 0);
    }
}