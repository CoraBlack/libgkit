#include "gkit/graphic/opengl/FrameBuffer.hpp"
#include "gkit/graphic/opengl/Texture.hpp"
#include "gkit/utils/log.hpp"

#include <glad/gl.h>

gkit::graphic::opengl::buffer::FrameBuffer::FrameBuffer(int width, int height)
	: fb_width(width), fb_height(height) {
	glGenFramebuffers(1, &this->renderer_id);
}

gkit::graphic::opengl::buffer::FrameBuffer::~FrameBuffer() {
	if(this->renderer_id != 0) {
		glDeleteFramebuffers(1, &this->renderer_id);
		this->renderer_id = 0;
	}
}

gkit::graphic::opengl::buffer::FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
    : renderer_id(other.renderer_id)
    , fb_height(other.fb_height)
    , fb_width(other.fb_width)
    , left_x(other.left_x)
    , bottom_y(other.bottom_y) {
    other.renderer_id = 0;
}

auto gkit::graphic::opengl::buffer::FrameBuffer::operator=(FrameBuffer&& other) noexcept -> FrameBuffer& {
    if (this != &other) {
        if (this->renderer_id != 0) {
            glDeleteFramebuffers(1, &this->renderer_id);
        }
        this->renderer_id = other.renderer_id;
        this->fb_height = other.fb_height;
        this->fb_width = other.fb_width;
        this->left_x = other.left_x;
        this->bottom_y = other.bottom_y;
        other.renderer_id = 0;
    }
    return *this;
}

auto gkit::graphic::opengl::buffer::FrameBuffer::attach_color_texture(const gkit::graphic::opengl::Texture& texture, int slot) -> void {
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, texture.get_renderer_id(), 0);
}

auto gkit::graphic::opengl::buffer::FrameBuffer::detach_color_texture(int slot) -> void {
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, 0, 0);
}

auto gkit::graphic::opengl::buffer::FrameBuffer::attach_depth_stencil(const RenderBuffer& rbo) -> void {
	bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER, rbo.get_render_id());
}

auto gkit::graphic::opengl::buffer::FrameBuffer::detach_depth_stencil() -> void {
	bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
								GL_RENDERBUFFER, 0);
}

auto gkit::graphic::opengl::buffer::FrameBuffer::check() -> void {
	bind();
	gkit::utils::Log::Message msg;
	msg.functions = static_cast<std::uint8_t>(gkit::utils::Log::LogFunction::Both);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		msg.level = gkit::utils::Log::LogLevel::Error;
		msg.message = "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
	} else {
		msg.level = gkit::utils::Log::LogLevel::Info;
		msg.message = "FRAMEBUFFER:: Framebuffer is complete!";
	}
	gkit::utils::Log::instance().log(std::move(msg));
	unbind();
}

auto gkit::graphic::opengl::buffer::FrameBuffer::set_viewport(int x, int y, int width, int height) -> void {
	glViewport(x, y, width, height);
}

auto gkit::graphic::opengl::buffer::FrameBuffer::set_viewport(int width, int height) -> void {
	glViewport(this->left_x, this->bottom_y, width, height);
}

auto gkit::graphic::opengl::buffer::FrameBuffer::set_viewport() -> void {
	glViewport(this->left_x, this->bottom_y, this->fb_width, this->fb_height);
}

auto gkit::graphic::opengl::buffer::FrameBuffer::bind() const -> void {
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id);
}

auto gkit::graphic::opengl::buffer::FrameBuffer::unbind() const -> void {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
