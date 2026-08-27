#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include <glad/glad.h>

#include <string_view>

namespace TextureUtils
{
    enum class TextureType 
    { 
        Texture2D, 
        Cubemap 
    };

    class Texture2D final
    {
    private:
        GLuint texture_id_ = 0;
        int width_ = 0;
        int height_ = 0;
        GLenum internal_format_ = 0;
        GLenum data_format_ = 0;
        TextureType type_;
        GLenum target_;

    public:
        Texture2D(
            std::string_view path, 
            bool sRGB, 
            bool generate_mipmaps, 
            bool flip_vertically, 
            GLenum wrap_s, 
            GLenum wrap_t, 
            GLenum min_filter, 
            GLenum mag_filter);

        Texture2D(
            std::string_view path, 
            GLsizei columns_n, 
            GLsizei rows_n, 
            const std::array<int, 6>& z_offsets, 
            bool sRGB, 
            bool generate_mipmaps, 
            bool flip_vertically, 
            GLenum wrap_s, 
            GLenum wrap_t, 
            GLenum wrap_r, 
            GLenum min_filter, 
            GLenum mag_filter);

        Texture2D(const Texture2D& other) = delete;
        Texture2D& operator=(const Texture2D& other) = delete;

        Texture2D(Texture2D&& other) noexcept;
        Texture2D& operator=(Texture2D&& other) noexcept;
        
        ~Texture2D();

        void Bind(GLuint unit_index = 0) const noexcept;
        void Unbind(GLuint unit_index = 0) const noexcept;

        // Getters
        GLuint Id() const noexcept { return texture_id_; }
        int Width() const noexcept { return width_; }
        int Height() const noexcept { return height_; }
        GLenum InternalFormat() const noexcept { return internal_format_; }
        GLenum DataFormat() const noexcept { return data_format_; }
        TextureType Type() const noexcept { return type_; }
        GLenum Target() const noexcept { return target_; }
    };

} // namespace TextureUtils

#endif // TEXTURE_2D_HPP
