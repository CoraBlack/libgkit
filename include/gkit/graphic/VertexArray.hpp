#pragma once

#include "gkit/graphic/VertexBuffer.hpp"
#include "gkit/graphic/VertexBufferLayout.hpp"

/**
 * @brief 顶点数组(前端抽象接口)
 *
 * 保存顶点缓冲与属性布局的绑定配置(OpenGL VAO / Vulkan pipeline 顶点输入)。
 */
namespace gkit::graphic {

    class VertexArray {
    public:
        VertexArray()                                  = default;
        VertexArray(const VertexArray&)                = delete;
        auto operator=(const VertexArray&) -> VertexArray& = delete;

        VertexArray(VertexArray&&) noexcept                    = default;
        auto operator=(VertexArray&&) noexcept -> VertexArray& = default;

        virtual ~VertexArray() = default;

        /// @brief 绑定一个顶点缓冲及其布局
        virtual auto add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) -> void = 0;

        /// @brief 添加实例化缓冲(逐实例数据)
        virtual auto add_instance_buffer(const VertexBuffer& vb) -> void = 0;

        /// @brief 绑定到当前后端上下文
        virtual auto bind() const -> void = 0;

        /// @brief 解绑
        virtual auto unbind() const -> void = 0;
    };

} // namespace gkit::graphic
