#include "gkit/graphic/opengl/IndexBuffer.hpp"

#include <glad/gl.h>

gkit::graphic::opengl::buffer::IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
    : count(count) {
    glGenBuffers(1, &this->renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}
gkit::graphic::opengl::buffer::IndexBuffer::~IndexBuffer() {
    if(this->renderer_id != 0) {
        glDeleteBuffers(1, &this->renderer_id);
        this->renderer_id = 0;
    }
}

gkit::graphic::opengl::buffer::IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : renderer_id(other.renderer_id)
    , count(other.count) {
    other.renderer_id = 0;
}

auto gkit::graphic::opengl::buffer::IndexBuffer::operator=(IndexBuffer&& other) noexcept -> IndexBuffer& {
    if (this != &other) {
        if (this->renderer_id != 0) {
            glDeleteBuffers(1, &this->renderer_id);
        }
        this->renderer_id = other.renderer_id;
        this->count = other.count;
        other.renderer_id = 0;
    }
    return *this;
}

auto gkit::graphic::opengl::buffer::IndexBuffer::bind() const -> void {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id);
}

auto gkit::graphic::opengl::buffer::IndexBuffer::unbind() const -> void {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
