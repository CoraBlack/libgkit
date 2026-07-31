#include "gkit/graphic/opengl/FrameBuffer.hpp"

#include "gkit/core/log.hpp"
#include "gkit/graphic/opengl/RenderBuffer.hpp"
#include "gkit/graphic/opengl/Texture.hpp"

#include <glad/gl.h>

namespace gkit::graphic::opengl {

    FrameBuffer::FrameBuffer(int width, int height) : fb_width(width), fb_height(height) {
        glGenFramebuffers(1, &this->renderer_id);
    }

    FrameBuffer::~FrameBuffer() {
        if (this->renderer_id != 0) {
            glDeleteFramebuffers(1, &this->renderer_id);
            this->renderer_id = 0;
        }
    }

    auto FrameBuffer::attach_color_texture(const graphic::Texture& texture, int slot) -> void {
        bind();
        const auto& gl_texture = static_cast<const gkit::graphic::opengl::Texture&>(texture);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, gl_texture.get_renderer_id(),
                               0);
    }

    auto FrameBuffer::detach_color_texture(int slot) -> void {
        bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, 0, 0);
    }

    auto FrameBuffer::attach_depth_stencil(const graphic::RenderBuffer& rbo) -> void {
        bind();
        const auto& gl_rbo = static_cast<const gkit::graphic::opengl::RenderBuffer&>(rbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                  gl_rbo.get_render_id());
    }

    auto FrameBuffer::detach_depth_stencil() -> void {
        bind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
    }

    auto FrameBuffer::check() -> void {
        bind();
        gkit::core::Log::Message msg;
        msg.functions = static_cast<std::uint8_t>(gkit::core::Log::LogFunction::Both);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            msg.level   = gkit::core::Log::LogLevel::Error;
            msg.message = "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
        } else {
            msg.level   = gkit::core::Log::LogLevel::Info;
            msg.message = "FRAMEBUFFER:: Framebuffer is complete!";
        }
        gkit::core::Log::instance().log(std::move(msg));
        unbind();
    }

    auto FrameBuffer::set_viewport(int x, int y, int width, int height) -> void {
        glViewport(x, y, width, height);
    }

    auto FrameBuffer::set_viewport(int width, int height) -> void {
        glViewport(this->left_x, this->bottom_y, width, height);
    }

    auto FrameBuffer::set_viewport() -> void {
        glViewport(this->left_x, this->bottom_y, this->fb_width, this->fb_height);
    }

    auto FrameBuffer::bind() const -> void {
        glBindFramebuffer(GL_FRAMEBUFFER, this->renderer_id);
    }

    auto FrameBuffer::unbind() const -> void {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

} // namespace gkit::graphic::opengl
