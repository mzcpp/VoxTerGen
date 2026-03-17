#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <glad/glad/glad.h>

#include <string_view>
#include <span>

namespace TextureUtils
{
    enum class TextureType 
    { 
        Texture2D, 
        CubeMap 
    };

    constexpr GLenum DEFAULT_WRAP_S = GL_REPEAT;
    constexpr GLenum DEFAULT_WRAP_T = GL_REPEAT;
    constexpr GLenum DEFAULT_WRAP_R = GL_REPEAT;
    constexpr GLenum DEFAULT_MIN_FILTER = GL_LINEAR_MIPMAP_LINEAR;
    constexpr GLenum DEFAULT_MAG_FILTER = GL_LINEAR;
    constexpr GLenum DEFAULT_WRAP_RGBA = GL_CLAMP_TO_EDGE;

    class Texture2D final
    {
    private:
        GLuint texture_id_ = 0;
        int width_ = 0;
        int height_ = 0;
        GLenum internal_format_ = 0;
        GLenum data_format_ = 0;
        TextureType type_ = TextureType::Texture2D;
        GLenum target_ = GL_TEXTURE_2D;

    public:
        Texture2D(std::string_view path, bool sRGB = false, bool generate_mipmaps = true,
            GLenum wrap_s = DEFAULT_WRAP_S, GLenum wrap_t = DEFAULT_WRAP_T,
            GLenum min_filter = DEFAULT_MIN_FILTER, GLenum mag_filter = DEFAULT_MAG_FILTER);

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;

        Texture2D(Texture2D&& other) noexcept;
        Texture2D& operator=(Texture2D&& other) noexcept;
        
        ~Texture2D();

        void Bind(GLuint unit_index = 0) const noexcept;
        void Unbind(GLuint unit_index = 0) const noexcept;

        GLuint Id() const noexcept { return texture_id_; }
        int Width() const noexcept { return width_; }
        int Height() const noexcept { return height_; }
        GLenum InternalFormat() const noexcept { return internal_format_; }
        GLenum DataFormat() const noexcept { return data_format_; }
        TextureType Type() const noexcept { return type_; }
        GLenum Target() const noexcept { return target_; }
    };

} // namespace TextureUtils

#endif
