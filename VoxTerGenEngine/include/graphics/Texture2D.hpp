#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include <glad/glad.h>

#include <string_view>
#include <span>

namespace TextureUtils
{
    /**
     * @brief Identifies the type of texture.
     *
     * Used to distinguish between standard 2D textures and cubemap textures.
     */
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

    /**
     * @brief Represents a 2D or cubemap texture in OpenGL.
     *
     * Texture2D can be created from an image file, raw pixel data, or a cubemap with 6 images.
     * Supports mipmap generation, sRGB conversion, and custom filtering and wrapping parameters.
     * Move-only class; copying is disallowed.
     */
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
        /**
         * @brief Constructs a 2D texture from an image file.
         *
         * @param path File path to the image.
         * @param sRGB If true, converts texture to sRGB format.
         * @param generate_mipmaps Whether to generate mipmaps.
         * @param wrap_s Wrapping mode for the S coordinate.
         * @param wrap_t Wrapping mode for the T coordinate.
         * @param min_filter Minification filter.
         * @param mag_filter Magnification filter.
         *
         * @throws std::runtime_error if loading or creating the texture fails.
         */
        Texture2D(std::string_view path, bool sRGB = false, bool generate_mipmaps = true,
            GLenum wrap_s = DEFAULT_WRAP_S, GLenum wrap_t = DEFAULT_WRAP_T,
            GLenum min_filter = DEFAULT_MIN_FILTER, GLenum mag_filter = DEFAULT_MAG_FILTER);

        /**
         * @brief Constructs a 2D texture from raw pixel data.
         *
         * @param width Texture width in pixels.
         * @param height Texture height in pixels.
         * @param data Pointer to the pixel data.
         * @param format OpenGL internal and data format.
         * @param generate_mipmaps Whether to generate mipmaps.
         * @param wrap_s Wrapping mode for the S coordinate.
         * @param wrap_t Wrapping mode for the T coordinate.
         * @param min_filter Minification filter.
         * @param mag_filter Magnification filter.
         *
         * @throws std::runtime_error if dimensions are invalid.
         */
        Texture2D(int width, int height, const unsigned char* data,
            GLenum format = GL_RGBA, bool generate_mipmaps = true,
            GLenum wrap_s = DEFAULT_WRAP_S, GLenum wrap_t = DEFAULT_WRAP_T,
            GLenum min_filter = DEFAULT_MIN_FILTER, GLenum mag_filter = DEFAULT_MAG_FILTER);

        /**
         * @brief Constructs a cubemap texture from 6 image files.
         *
         * @param paths Array of 6 image file paths (order: +X, -X, +Y, -Y, +Z, -Z).
         * @param sRGB If true, converts texture to sRGB format.
         * @param generate_mipmaps Whether to generate mipmaps.
         * @param wrap_s Wrapping mode for the S coordinate.
         * @param wrap_t Wrapping mode for the T coordinate.
         * @param wrap_r Wrapping mode for the R coordinate.
         * @param min_filter Minification filter.
         * @param mag_filter Magnification filter.
         *
         * @throws std::runtime_error if loading fails, or cubemap faces mismatch.
         */
        Texture2D(std::span<std::string_view> paths, bool sRGB = false, bool generate_mipmaps = true,
            GLenum wrap_s = DEFAULT_WRAP_R, GLenum wrap_t = DEFAULT_WRAP_R, GLenum wrap_r = DEFAULT_WRAP_R,
            GLenum min_filter = DEFAULT_MIN_FILTER, GLenum mag_filter = DEFAULT_MAG_FILTER);

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;

        /** @brief Move constructor. Transfers ownership from another texture. */
        Texture2D(Texture2D&& other) noexcept;

        /** @brief Move assignment operator. Transfers ownership from another texture. */
        Texture2D& operator=(Texture2D&& other) noexcept;

        /** @brief Destructor. Deletes the OpenGL texture object if valid. */
        ~Texture2D();

        /** @brief Binds the texture to a given texture unit. */
        void Bind(GLuint unit_index = 0) const noexcept;

        /** @brief Unbinds the texture from a given texture unit. */
        void Unbind(GLuint unit_index = 0) const noexcept;

        // Getters
        /** @brief Returns the OpenGL texture ID. */
        GLuint Id() const noexcept { return texture_id_; }

        /** @brief Returns the texture width in pixels. */
        int Width() const noexcept { return width_; }

        /** @brief Returns the texture height in pixels. */
        int Height() const noexcept { return height_; }

        /** @brief Returns the internal OpenGL format of the texture. */
        GLenum InternalFormat() const noexcept { return internal_format_; }

        /** @brief Returns the OpenGL data format of the texture. */
        GLenum DataFormat() const noexcept { return data_format_; }

        /** @brief Returns the type of texture (2D or CubeMap). */
        TextureType Type() const noexcept { return type_; }

        /** @brief Returns the OpenGL target of the texture (GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP). */
        GLenum Target() const noexcept { return target_; }
    };

} // namespace TextureUtils

#endif // TEXTURE_2D_HPP