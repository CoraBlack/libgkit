#include "gkit/graphic/opengl/IndexBuffer.hpp"

#include <glad/gl.h>

namespace gkit::graphic::opengl {

    IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count) {
        glGenBuffers(1, &this->renderer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
        this->count = count;
        this->size  = count * sizeof(uint32_t);
    }

    IndexBuffer::~IndexBuffer() {
        if (this->renderer_id != 0) {
            glDeleteBuffers(1, &this->renderer_id);
            this->renderer_id = 0;
        }
    }

    auto IndexBuffer::bind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id);
    }

    auto IndexBuffer::unbind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    auto IndexBuffer::update_data(const void* data, uint32_t size) -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id);
        if (size == this->size) {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
        } else {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
            this->size = size;
        }
    }

    auto IndexBuffer::update_sub_data(uint32_t offset, const void* data, uint32_t size) -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    }

} // namespace gkit::graphic::opengl
