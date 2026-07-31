#pragma once

#include "gkit/graphic/VertexBuffer.hpp"

#include <cstdint>

/**
 * @brief OpenGL 后端顶点缓冲(VBO)
 *
 * 继承前端 `graphic::VertexBuffer`, 实现具体的 GL 缓冲操作。
 */
namespace gkit::graphic::opengl {

    class VertexBuffer final : public graphic::VertexBuffer {
    public:
        /**
		 * @brief 构造顶点缓冲
		 * @param data 顶点数据指针
		 * @param size 数据字节数
		 * @param dynamic true 则频繁更新(GL_DYNAMIC_DRAW)
		 */
        explicit VertexBuffer(const void* data, uint32_t size, bool dynamic = false);

        ~VertexBuffer() override;

        auto bind() const -> void override;
        auto unbind() const -> void override;
        auto update_data(const void* data, uint32_t size) -> void override;
        auto update_sub_data(uint32_t offset, const void* data, uint32_t size) -> void override;
        [[nodiscard]] auto is_dynamic() const -> bool override { return this->dynamic; }

    private:
        uint32_t renderer_id = 0; // GL 缓冲句柄
        bool dynamic         = false;
    };

} // namespace gkit::graphic::opengl
