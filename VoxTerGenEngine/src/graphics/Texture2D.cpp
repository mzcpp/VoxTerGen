#include "graphics/Texture2D.hpp"
#include "utils/Logger.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad/glad.h>

#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <cmath>
#include <string_view>
#include <cstring>

namespace TextureUtils
{
    static void GetTextureFormats(int n_components, bool sRGB, GLenum* internal_format, GLenum* data_format)
    {
        switch (n_components)
        {
        case 1:
            *internal_format = GL_R8;
            *data_format = GL_RED;
            break;
        case 2:
            *internal_format = GL_RG8;
            *data_format = GL_RG;
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
            Logger::Log(LogLevel::INFO, "Unsupported texture channel count. Current count: {}", n_components);
            throw std::runtime_error("Unsupported texture channel count");
        }
    }

    Texture2D::Texture2D(
        std::string_view path, 
        bool sRGB, 
        bool generate_mipmaps, 
        GLenum wrap_s, 
        GLenum wrap_t, 
        GLenum min_filter, 
        GLenum mag_filter) 
        : 
        type_(TextureType::Texture2D), 
        target_(GL_TEXTURE_2D)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &texture_id_);
        int n_components = 0;
        
        std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> data = { stbi_load(path.data(), &width_, &height_, &n_components, 0), stbi_image_free };

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

        GetTextureFormats(n_components, sRGB, &internal_format_, &data_format_);

        const int levels = generate_mipmaps ? 1 + static_cast<int>(std::floor(std::log2(std::max(width_, height_)))) : 1;
        
        GLint previous_unpack_alignment = 0;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &previous_unpack_alignment);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        glTextureStorage2D(texture_id_, levels, internal_format_, width_, height_);
        glTextureSubImage2D(texture_id_, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data.get());

        if (generate_mipmaps)
        {
            glGenerateTextureMipmap(texture_id_);
        }
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, previous_unpack_alignment);
        
        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_S, wrap_s);
        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_T, wrap_t);
        glTextureParameteri(texture_id_, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(texture_id_, GL_TEXTURE_MAG_FILTER, mag_filter);
    }

    Texture2D::Texture2D(
        std::string_view path,
        bool sRGB, 
        bool generate_mipmaps, 
        GLenum wrap_s, 
        GLenum wrap_t, 
        GLenum wrap_r, 
        GLenum min_filter, 
        GLenum mag_filter) 
        : 
        type_(TextureType::Cubemap), 
        target_(GL_TEXTURE_CUBE_MAP)
    {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &texture_id_);
        int n_components = 0;
        
        std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> data = { stbi_load(path.data(), &width_, &height_, &n_components, 0), stbi_image_free };

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
        
        GetTextureFormats(n_components, sRGB, &internal_format_, &data_format_);
        
        const int levels = generate_mipmaps ? 1 + static_cast<int>(std::floor(std::log2(std::max(width_, height_)))) : 1;
        
        GLint previous_unpack_alignment = 0;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &previous_unpack_alignment);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        constexpr int face_image_dimension = 512;
        std::unique_ptr<stbi_uc[]> buffer = std::make_unique<stbi_uc[]>(face_image_dimension * face_image_dimension * n_components);

        glTextureStorage2D(texture_id_, levels, internal_format_, face_image_dimension, face_image_dimension);

        //for (int col = 0, int face_n = 0, int data_offset = 0; col < 6; ++col, ++face_n)
        //{
        //    // copy 512 rows
        //    for (int row = 0; row < face_image_dimension; ++row)
        //    {
        //        std::memcpy(buffer.get(), data.get() + data_offset, face_image_dimension);
        //    }

        //    //glTextureSubImage3D(texture_id_, 0, 0, 0, face_n, face_image_dimension, face_image_dimension, data_format_, GL_UNSIGNED_BYTE, buffer.get());

        //    data_offset += face_image_dimension;
        //}

        if (generate_mipmaps)
        {
            glGenerateTextureMipmap(texture_id_);
        }
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, previous_unpack_alignment);
        
        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_S, wrap_s);
        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_T, wrap_t);
        glTextureParameteri(texture_id_, GL_TEXTURE_WRAP_R, wrap_r);
        glTextureParameteri(texture_id_, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(texture_id_, GL_TEXTURE_MAG_FILTER, mag_filter);
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