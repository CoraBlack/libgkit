#include "gkit/graphic/opengl/RenderBuffer.hpp"

#include <glad/gl.h>

gkit::graphic::opengl::buffer::RenderBuffer::RenderBuffer(int width, int height) {
	glGenRenderbuffers(1, &this->renderer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, this->renderer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	unbind();
}

gkit::graphic::opengl::buffer::RenderBuffer::~RenderBuffer() {
	if(this->renderer_id != 0) {
		glDeleteRenderbuffers(1, &this->renderer_id);
		this->renderer_id = 0;
	}
}

gkit::graphic::opengl::buffer::RenderBuffer::RenderBuffer(RenderBuffer&& other) noexcept
    : renderer_id(other.renderer_id) {
    other.renderer_id = 0;
}

auto gkit::graphic::opengl::buffer::RenderBuffer::operator=(RenderBuffer&& other) noexcept -> RenderBuffer& {
    if (this != &other) {
        if (this->renderer_id != 0) {
            glDeleteRenderbuffers(1, &this->renderer_id);
        }
        this->renderer_id = other.renderer_id;
        other.renderer_id = 0;
    }
    return *this;
}

auto gkit::graphic::opengl::buffer::RenderBuffer::bind() const -> void {
	glBindRenderbuffer(GL_RENDERBUFFER, this->renderer_id);
}

auto gkit::graphic::opengl::buffer::RenderBuffer::unbind() const -> void {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
