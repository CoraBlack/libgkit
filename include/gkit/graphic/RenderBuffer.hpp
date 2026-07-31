#pragma once

namespace gkit::graphic {

    /**
	 * @brief 渲染缓冲(前端抽象接口)
	 *
	 * 离屏渲染缓冲(OpenGL renderbuffer), 不能被着色器直接采样,
	 * 通常用于帧缓冲的深度/模板附件。
	 * 不属于 Buffer 家族(它是渲染目标附件, 而非数据缓冲)。
	 */
    class RenderBuffer {
    public:
        RenderBuffer()                                       = default;
        RenderBuffer(const RenderBuffer&)                    = delete;
        auto operator=(const RenderBuffer&) -> RenderBuffer& = delete;

        RenderBuffer(RenderBuffer&&) noexcept                    = default;
        auto operator=(RenderBuffer&&) noexcept -> RenderBuffer& = default;

        virtual ~RenderBuffer() = default;

        /// @brief 绑定到当前后端上下文
        virtual auto bind() const -> void = 0;

        /// @brief 解绑
        virtual auto unbind() const -> void = 0;
    };

} // namespace gkit::graphic
