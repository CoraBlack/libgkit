#pragma once

#include "gkit/graphic/Buffer.hpp"

#include <cstdint>

namespace gkit::graphic {

    /**
	 * @brief 索引数据缓冲(前端抽象接口)
	 *
	 * 存储绘制顺序的索引数据, 用于高效绘制索引几何体。
	 */
    class IndexBuffer : public Buffer {
    public:
        ~IndexBuffer() override = default;

        /// @brief 索引数量
        [[nodiscard]] virtual auto get_count() const -> uint32_t = 0;
    };

} // namespace gkit::graphic
