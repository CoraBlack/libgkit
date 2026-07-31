#pragma once

#include "gkit/graphic/Buffer.hpp"

namespace gkit::graphic {

    /**
	 * @brief 存储缓冲(占位, 暂不实现)
	 *
	 * 供计算着色器读写(SSBO)的缓冲。资源模块与后端就绪后,
	 * 在此基类上扩展(绑定到计算管线/材质槽位)。
	 *
	 * TODO(将来): 实现后端(opengl::StorageBuffer)与 Device 工厂方法
	 *             create_storage_buffer()。
	 */
    // class StorageBuffer : public Buffer {
    // public:
    //     ~StorageBuffer() override = default;
    // };

} // namespace gkit::graphic
