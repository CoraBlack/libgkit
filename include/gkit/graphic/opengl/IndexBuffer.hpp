#pragma once

#include "gkit/graphic/IndexBuffer.hpp"

#include <cstdint>

/**
 * @brief OpenGL 后端索引缓冲(EBO)
 *
 * 继承前端 `graphic::IndexBuffer`, 实现具体的 GL 缓冲操作。
 */
namespace gkit::graphic::opengl {

    class IndexBuffer final : public graphic::IndexBuffer {
    public:
        /**
		 * @brief 构造索引缓冲
		 * @param data 索引数据指针
		 * @param count 索引数量
		 */
        explicit IndexBuffer(const uint32_t* data, uint32_t count);

        ~IndexBuffer() override;

        auto bind() const -> void override;
        auto unbind() const -> void override;
        auto update_data(const void* data, uint32_t size) -> void override;
        auto update_sub_data(uint32_t offset, const void* data, uint32_t size) -> void override;
        [[nodiscard]] auto get_count() const -> uint32_t override { return this->count; }

    private:
        uint32_t renderer_id = 0; // GL 缓冲句柄
        uint32_t count       = 0; // 索引数量
    };

} // namespace gkit::graphic::opengl
