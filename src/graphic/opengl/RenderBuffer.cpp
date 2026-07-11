#include "gkit/graphic/opengl/RenderBuffer.hpp"

#include <glad/gl.h>

gkit::graphic::opengl::buffer::RenderBuffer::RenderBuffer(int width, int height) {
	glGenRenderbuffers(1, &m_renderer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	unbind();
}

gkit::graphic::opengl::buffer::RenderBuffer::~RenderBuffer() {
	if(m_renderer_id != 0) {
		glDeleteRenderbuffers(1, &m_renderer_id);
		m_renderer_id = 0;
	}
}

gkit::graphic::opengl::buffer::RenderBuffer::RenderBuffer(RenderBuffer&& other) noexcept
    : m_renderer_id(other.m_renderer_id) {
    other.m_renderer_id = 0;
}

auto gkit::graphic::opengl::buffer::RenderBuffer::operator=(RenderBuffer&& other) noexcept -> RenderBuffer& {
    if (this != &other) {
        if (m_renderer_id != 0) {
            glDeleteRenderbuffers(1, &m_renderer_id);
        }
        m_renderer_id = other.m_renderer_id;
        other.m_renderer_id = 0;
    }
    return *this;
}

auto gkit::graphic::opengl::buffer::RenderBuffer::bind() const -> void {
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderer_id);
}

auto gkit::graphic::opengl::buffer::RenderBuffer::unbind() const -> void {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}