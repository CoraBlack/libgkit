#pragma once

#include "gkit/graphic/Buffer.hpp"

namespace gkit::graphic {

    /**
	 * @brief Uniform 数据缓冲(占位, 暂不实现)
	 *
	 * 存储 uniform 数据的缓冲(UBO), 用于批量设置着色器常量。
	 *
	 * TODO(将来): 实现后端(opengl::UniformBuffer)与 Device 工厂方法
	 *             create_uniform_buffer()。
	 */
    // class UniformBuffer : public Buffer {
    // public:
    //     ~UniformBuffer() override = default;
    // };

} // namespace gkit::graphic
