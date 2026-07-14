#pragma once

#include "gkit/graphic/opengl/config.hpp"

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Texture wrapper for OpenGL texture objects
 *
 * A Texture represents image data that can be sampled by shaders.
 * Supports 2D textures, cube maps, and framebuffer textures.
 */
namespace gkit::graphic::opengl {

    class Texture {
    public:
        Texture(const Texture&)                    = delete;
        auto operator=(const Texture&) -> Texture& = delete;

        /** @brief Move constructor - transfers ownership of GL texture
		 *  @param other Source object to move from (will be invalidated)
		 */
        Texture(Texture&& other) noexcept;

        /** @brief Move assignment - transfers ownership of GL texture
		 *  @param other Source object to move from (will be invalidated)
		 *  @note Releases any existing GL texture before taking ownership
		 */
        auto operator=(Texture&& other) noexcept -> Texture&;

    public:
        /**
		 * @brief Construct a texture (deprecated)
		 * @param path Path to the texture file (can be empty for framebuffer textures)
		 * @param type Texture type (Texture2D, TextureCubeMap, TextureFramebuffer)
		 * @deprecated Use TextureType enum to specify texture type instead
		 */
        [[deprecated("In the future, the texture resources provided by the resource management module will be used. "
                     "Currently, they are merely serving as placeholders.")]]
        explicit Texture(const std::string& path, TextureType type = TextureType::Texture2D);

        /**
		 * @brief Destructor - deletes the texture
		 */
        ~Texture();

        /**
		 * @brief Bind this texture to a specific slot
		 * @param slot Texture slot (0-15)
		 */
        auto bind(unsigned int slot = 0) const -> void;

        /**
		 * @brief Unbind this texture
		 */
        auto unbind() const -> void;

        /**
		 * @brief Get the texture width
		 * @return Width in pixels
		 */
        [[nodiscard]] inline auto get_width() const -> int { return this->width; }

        /**
		 * @brief Get the texture height
		 * @return Height in pixels
		 */
        [[nodiscard]] inline auto get_height() const -> int { return this->height; }

        /**
		 * @brief Get the Render ID object
		 * @return ID
		 */
        [[nodiscard]] inline auto get_renderer_id() const -> uint32_t { return this->renderer_id; }

    private:
        /**
		 * @brief Cube map face file names
		 */
        inline static const std::vector<std::string> FACES = {
            "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};

    private:
        uint32_t renderer_id; // OpenGL texture ID
        std::string filepath; // Path to the texture file
        unsigned char* local_buffer; // Local buffer for texture data
        int width; // Texture width in pixels
        int height; // Texture height in pixels
        int bpp; // Bits per pixel
        TextureType type; // Texture type (2D, CubeMap, FrameBuffer)
    };

} // namespace gkit::graphic::opengl
