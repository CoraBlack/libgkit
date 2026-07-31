#include "gkit/graphic/opengl/VertexBuffer.hpp"

#include <glad/gl.h>

namespace gkit::graphic::opengl {

    VertexBuffer::VertexBuffer(const void* data, uint32_t size, bool dynamic) {
        glGenBuffers(1, &this->renderer_id);
        glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id);
        if (!dynamic) {
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        } else {
            glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        }
        this->size    = size;
        this->dynamic = dynamic;
    }

    VertexBuffer::~VertexBuffer() {
        if (this->renderer_id != 0) {
            glDeleteBuffers(1, &this->renderer_id);
            this->renderer_id = 0;
        }
    }

    auto VertexBuffer::bind() const -> void {
        glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id);
    }

    auto VertexBuffer::unbind() const -> void {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    auto VertexBuffer::update_data(const void* data, uint32_t size) -> void {
        glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id);
        if (size == this->size) {
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        } else {
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
            this->size = size;
        }
    }

    auto VertexBuffer::update_sub_data(uint32_t offset, const void* data, uint32_t size) -> void {
        glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

} // namespace gkit::graphic::opengl
