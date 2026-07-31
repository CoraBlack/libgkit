#pragma once

#include "gkit/graphic/Buffer.hpp"

namespace gkit::graphic {

    /**
	 * @brief 顶点数据缓冲(前端抽象接口)
	 *
	 * 存储顶点数据(position/color/uv 等)到 GPU 的缓冲区。
	 */
    class VertexBuffer : public Buffer {
    public:
        ~VertexBuffer() override = default;

        /// @brief 是否为动态缓冲(频繁更新, 后端映射 GL_DYNAMIC_DRAW 等)
        [[nodiscard]] virtual auto is_dynamic() const -> bool = 0;
    };

} // namespace gkit::graphic
