#pragma once

#include "gkit/graphic/RenderBuffer.hpp"
#include "gkit/graphic/Texture.hpp"

namespace gkit::graphic {

    /**
	 * @brief 帧缓冲(前端抽象接口)
	 *
	 * 离屏渲染目标: 组合颜色纹理附件 + 深度/模板附件。
	 * 不属于 Buffer 家族(它组合了纹理与渲染缓冲, 而非数据缓冲)。
	 */
    class FrameBuffer {
    public:
        FrameBuffer()                                  = default;
        FrameBuffer(const FrameBuffer&)                = delete;
        auto operator=(const FrameBuffer&) -> FrameBuffer& = delete;

        FrameBuffer(FrameBuffer&&) noexcept                        = default;
        auto operator=(FrameBuffer&&) noexcept -> FrameBuffer& = default;

        virtual ~FrameBuffer() = default;

        /// @brief 附加颜色纹理到指定槽位
        virtual auto attach_color_texture(const Texture& texture, int slot) -> void = 0;

        /// @brief 解绑指定槽位的颜色纹理
        virtual auto detach_color_texture(int slot) -> void = 0;

        /// @brief 附加深度模板渲染缓冲
        virtual auto attach_depth_stencil(const RenderBuffer& rbo) -> void = 0;

        /// @brief 解绑深度模板附件
        virtual auto detach_depth_stencil() -> void = 0;

        /// @brief 检查帧缓冲完整性
        virtual auto check() -> void = 0;

        /// @brief 设置视口到帧缓冲默认尺寸
        virtual auto set_viewport() -> void = 0;

        /// @brief 设置自定义视口尺寸
        virtual auto set_viewport(int width, int height) -> void = 0;

        /// @brief 设置带偏移的自定义视口
        virtual auto set_viewport(int x, int y, int width, int height) -> void = 0;

        /// @brief 绑定为当前渲染目标
        virtual auto bind() const -> void = 0;

        /// @brief 解绑, 回到默认帧缓冲(屏幕)
        virtual auto unbind() const -> void = 0;
    };

} // namespace gkit::graphic
