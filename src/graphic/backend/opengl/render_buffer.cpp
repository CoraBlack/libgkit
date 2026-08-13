#include "graphic/backend/opengl/render_buffer.hpp"

#include <utility>

#include <glad/gl.h>

namespace gkit::graphic::opengl {

    RenderBuffer::RenderBuffer(int width, int height) {
        glGenRenderbuffers(1, &this->renderer_id);
        glBindRenderbuffer(GL_RENDERBUFFER, this->renderer_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        unbind();
    }

    RenderBuffer::RenderBuffer(RenderBuffer&& other) noexcept :
        graphic::RenderBuffer(std::move(other)), renderer_id(other.renderer_id) {
        other.renderer_id = 0;
    }

    auto RenderBuffer::operator=(RenderBuffer&& other) noexcept -> RenderBuffer& {
        if (this != &other) {
            if (this->renderer_id != 0) {
                glDeleteRenderbuffers(1, &this->renderer_id);
            }
            graphic::RenderBuffer::operator=(std::move(other));
            this->renderer_id = other.renderer_id;
            other.renderer_id = 0;
        }
        return *this;
    }

    RenderBuffer::~RenderBuffer() {
        if (this->renderer_id != 0) {
            glDeleteRenderbuffers(1, &this->renderer_id);
            this->renderer_id = 0;
        }
    }

    auto RenderBuffer::bind() const -> void {
        glBindRenderbuffer(GL_RENDERBUFFER, this->renderer_id);
    }

    auto RenderBuffer::unbind() const -> void {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

} // namespace gkit::graphic::opengl
