#pragma once

#include <cstdint>

/**
 * @brief 缓冲区基类(前端抽象接口)
 *
 * 所有 GPU 数据缓冲(顶点/索引等)共有的数据操作接口。
 * 具体后端(OpenGL/Vulkan)继承并实现 bind/unbind/update 等操作,
 * 资源句柄(renderer_id)只存在于后端实现内部。
 */
namespace gkit::graphic {

    class Buffer {
    public:
        Buffer()                                 = default;
        Buffer(const Buffer&)                    = delete;
        auto operator=(const Buffer&) -> Buffer& = delete;

        Buffer(Buffer&&) noexcept                    = default;
        auto operator=(Buffer&&) noexcept -> Buffer& = default;

        virtual ~Buffer() = default;

        /// @brief 绑定到当前后端上下文
        virtual auto bind() const -> void = 0;

        /// @brief 解绑
        virtual auto unbind() const -> void = 0;

        /// @brief 全量更新: 仅当 size 不变时后端走 SubData, 否则重建
        virtual auto update_data(const void* data, uint32_t size) -> void = 0;

        /// @brief 局部更新: 从 offset 起更新 size 字节
        virtual auto update_sub_data(uint32_t offset, const void* data, uint32_t size) -> void = 0;

        /// @brief 缓冲区大小(字节)
        [[nodiscard]] auto get_size() const -> uint32_t { return size; }

    protected:
        uint32_t size = 0; // 字节数
    };

} // namespace gkit::graphic
