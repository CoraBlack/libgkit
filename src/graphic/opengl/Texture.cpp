#include "gkit/graphic/opengl/Texture.hpp"

//#include <stb_image.h>
#include <glad/gl.h>

//#include <iostream>

gkit::graphic::opengl::Texture::Texture(const std::string& path, TextureType type)
	:m_renderer_id(0), m_filepath(path), m_localbuffer(nullptr), 
	m_width(0), m_height(0), m_bpp(0), m_type(type) {

}

gkit::graphic::opengl::Texture::~Texture() {
    if (m_renderer_id != 0) {
        glDeleteTextures(1, &m_renderer_id);
        m_renderer_id = 0;
    }
}

gkit::graphic::opengl::Texture::Texture(Texture&& other) noexcept
    : m_renderer_id(other.m_renderer_id)
    , m_filepath(std::move(other.m_filepath))
    , m_localbuffer(other.m_localbuffer)
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_bpp(other.m_bpp)
    , m_type(other.m_type) {
    other.m_renderer_id = 0;
    other.m_localbuffer = nullptr;
}

auto gkit::graphic::opengl::Texture::operator=(Texture&& other) noexcept -> Texture& {
    if (this != &other) {
        glDeleteTextures(1, &m_renderer_id);
        m_renderer_id = other.m_renderer_id;
        m_filepath = std::move(other.m_filepath);
        m_localbuffer = other.m_localbuffer;
        m_width = other.m_width;
        m_height = other.m_height;
        m_bpp = other.m_bpp;
        m_type = other.m_type;
        other.m_renderer_id = 0;
        other.m_localbuffer = nullptr;
    }
    return *this;
}

auto gkit::graphic::opengl::Texture::bind(unsigned int slot) const -> void {
	if (m_type == TextureType::TEXTURE_CUBE_MAP) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_renderer_id);
	} else if (m_type == TextureType::TEXTURE_2D || m_type == TextureType::TEXTURE_FRAMEBUFFER) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_renderer_id);
	}
}

auto gkit::graphic::opengl::Texture::unbind() const -> void {
	if (m_type == TextureType::TEXTURE_CUBE_MAP) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	} else if (m_type == TextureType::TEXTURE_2D || m_type == TextureType::TEXTURE_FRAMEBUFFER) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}