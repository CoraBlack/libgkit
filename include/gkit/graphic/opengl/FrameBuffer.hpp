#pragma once

#include "gkit/graphic/FrameBuffer.hpp"
#include "gkit/graphic/RenderBuffer.hpp"
#include "gkit/graphic/Texture.hpp"

#include <cstdint>

/**
 * @brief OpenGL 后端帧缓冲(FBO)
 *
 * 继承前端 `graphic::FrameBuffer`, 实现 GL 离屏渲染目标。
 */
namespace gkit::graphic::opengl {

    class FrameBuffer final : public graphic::FrameBuffer {
    public:
        explicit FrameBuffer(int width, int height);

        ~FrameBuffer() override;

        auto attach_color_texture(const graphic::Texture& texture, int slot) -> void override;
        auto detach_color_texture(int slot) -> void override;
        auto attach_depth_stencil(const graphic::RenderBuffer& rbo) -> void override;
        auto detach_depth_stencil() -> void override;
        auto check() -> void override;
        auto set_viewport() -> void override;
        auto set_viewport(int width, int height) -> void override;
        auto set_viewport(int x, int y, int width, int height) -> void override;
        auto bind() const -> void override;
        auto unbind() const -> void override;

    private:
        uint32_t renderer_id = 0; // GL 帧缓冲句柄
        unsigned int fb_height; // 帧缓冲高度
        unsigned int fb_width; // 帧缓冲宽度
        unsigned int left_x   = 0; // 视口左坐标
        unsigned int bottom_y = 0; // 视口底坐标
    };

} // namespace gkit::graphic::opengl
