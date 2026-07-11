#include "gkit/graphic/opengl/IndexBuffer.hpp"

#include <glad/gl.h>

gkit::graphic::opengl::buffer::IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
    : m_count(count) {
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}
gkit::graphic::opengl::buffer::IndexBuffer::~IndexBuffer() {
    if(m_renderer_id != 0) {
        glDeleteBuffers(1, &m_renderer_id);
        m_renderer_id = 0;
    }
}

gkit::graphic::opengl::buffer::IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_renderer_id(other.m_renderer_id)
    , m_count(other.m_count) {
    other.m_renderer_id = 0;
}

auto gkit::graphic::opengl::buffer::IndexBuffer::operator=(IndexBuffer&& other) noexcept -> IndexBuffer& {
    if (this != &other) {
        if (m_renderer_id != 0) {
            glDeleteBuffers(1, &m_renderer_id);
        }
        m_renderer_id = other.m_renderer_id;
        m_count = other.m_count;
        other.m_renderer_id = 0;
    }
    return *this;
}

auto gkit::graphic::opengl::buffer::IndexBuffer::bind() const -> void {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
}

auto gkit::graphic::opengl::buffer::IndexBuffer::unbind() const -> void {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}