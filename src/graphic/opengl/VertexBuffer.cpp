#include "gkit/graphic/opengl/VertexBuffer.hpp"

#include <glad/gl.h>

gkit::graphic::opengl::buffer::VertexBuffer::VertexBuffer(const void* data, uint32_t size, bool dynamic) {
    glGenBuffers(1, &this->renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id);
    if (!dynamic) {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }
    this->size = size;
}

gkit::graphic::opengl::buffer::VertexBuffer::~VertexBuffer() {
    if (this->renderer_id != 0) {
        glDeleteBuffers(1, &this->renderer_id);
        this->renderer_id = 0;
    }
}

gkit::graphic::opengl::buffer::VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept :
    renderer_id(other.renderer_id), size(other.size) {
    other.renderer_id = 0;
}

auto gkit::graphic::opengl::buffer::VertexBuffer::operator=(VertexBuffer&& other) noexcept -> VertexBuffer& {
    if (this != &other) {
        if (this->renderer_id != 0) {
            glDeleteBuffers(1, &this->renderer_id);
        }
        this->renderer_id = other.renderer_id;
        this->size        = other.size;
        other.renderer_id = 0;
    }
    return *this;
}

auto gkit::graphic::opengl::buffer::VertexBuffer::update_data(const void* data, uint32_t size) -> void {
    glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id);
    if (size == this->size) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    } else {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
        this->size = size;
    }
}

auto gkit::graphic::opengl::buffer::VertexBuffer::update_sub_data(uint32_t offset, const void* data, uint32_t size)
    -> void {
    glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

auto gkit::graphic::opengl::buffer::VertexBuffer::bind() const -> void {
    glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id);
}

auto gkit::graphic::opengl::buffer::VertexBuffer::unbind() const -> void {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
