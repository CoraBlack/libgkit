#include "gkit/graphic/opengl/Texture.hpp"

//#include <stb_image.h>
#include <glad/gl.h>

//#include <iostream>

gkit::graphic::opengl::Texture::Texture(const std::string& path, TextureType type) :
    renderer_id(0), filepath(path), local_buffer(nullptr), width(0), height(0), bpp(0), type(type) {}

gkit::graphic::opengl::Texture::~Texture() {
    delete[] this->local_buffer;
    if (this->renderer_id != 0) {
        glDeleteTextures(1, &this->renderer_id);
        this->renderer_id = 0;
    }
}

gkit::graphic::opengl::Texture::Texture(Texture&& other) noexcept :
    renderer_id(other.renderer_id), filepath(std::move(other.filepath)), local_buffer(other.local_buffer),
    width(other.width), height(other.height), bpp(other.bpp), type(other.type) {
    other.renderer_id  = 0;
    other.local_buffer = nullptr;
}

auto gkit::graphic::opengl::Texture::operator=(Texture&& other) noexcept -> Texture& {
    if (this != &other) {
        glDeleteTextures(1, &this->renderer_id);
        this->renderer_id  = other.renderer_id;
        this->filepath     = std::move(other.filepath);
        this->local_buffer = other.local_buffer;
        this->width        = other.width;
        this->height       = other.height;
        this->bpp          = other.bpp;
        this->type         = other.type;
        other.renderer_id  = 0;
        other.local_buffer = nullptr;
    }
    return *this;
}

auto gkit::graphic::opengl::Texture::bind(unsigned int slot) const -> void {
    if (this->type == TextureType::TextureCubeMap) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->renderer_id);
    } else if (this->type == TextureType::Texture2D || this->type == TextureType::TextureFramebuffer) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, this->renderer_id);
    }
}

auto gkit::graphic::opengl::Texture::unbind() const -> void {
    if (this->type == TextureType::TextureCubeMap) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    } else if (this->type == TextureType::Texture2D || this->type == TextureType::TextureFramebuffer) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
