#pragma once

#include "gkit/graphic/RenderBuffer.hpp"

#include <cstdint>

/**
 * @brief OpenGL 后端渲染缓冲(renderbuffer)
 *
 * 继承前端 `graphic::RenderBuffer`, 用于帧缓冲的深度/模板附件。
 */
namespace gkit::graphic::opengl {

    class RenderBuffer final : public graphic::RenderBuffer {
    public:
        explicit RenderBuffer(int width, int height);

        ~RenderBuffer() override;

        auto bind() const -> void override;
        auto unbind() const -> void override;

        /// @brief 获取 GL 渲染缓冲句柄(后端逃生通道)
        [[nodiscard]] auto get_render_id() const -> uint32_t { return this->renderer_id; }

    private:
        uint32_t renderer_id = 0; // GL 渲染缓冲句柄
    };

} // namespace gkit::graphic::opengl
