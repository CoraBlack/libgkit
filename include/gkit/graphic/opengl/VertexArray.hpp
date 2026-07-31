#pragma once

#include "gkit/graphic/VertexArray.hpp"
#include "gkit/graphic/VertexBuffer.hpp"
#include "gkit/graphic/VertexBufferLayout.hpp"

#include <cstdint>

/**
 * @brief OpenGL 后端顶点数组(VAO)
 *
 * 继承前端 `graphic::VertexArray`, 维护 GL 顶点属性配置。
 */
namespace gkit::graphic::opengl {

    class VertexArray final : public graphic::VertexArray {
    public:
        VertexArray();

        ~VertexArray() override;

        auto add_buffer(const graphic::VertexBuffer& vb, const graphic::VertexBufferLayout& layout) -> void override;
        auto add_instance_buffer(const graphic::VertexBuffer& vb) -> void override;
        auto bind() const -> void override;
        auto unbind() const -> void override;

    private:
        uint32_t renderer_id  = 0; // GL 顶点数组句柄
        uint32_t attrib_index = 0; // 当前属性索引
    };

} // namespace gkit::graphic::opengl
