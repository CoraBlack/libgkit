#include "gkit/graphic/opengl/RenderBuffer.hpp"

#include <glad/gl.h>

namespace gkit::graphic::opengl {

    RenderBuffer::RenderBuffer(int width, int height) {
        glGenRenderbuffers(1, &this->renderer_id);
        glBindRenderbuffer(GL_RENDERBUFFER, this->renderer_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        unbind();
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
