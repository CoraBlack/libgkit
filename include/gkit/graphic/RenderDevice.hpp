#pragma once

#include "gkit/graphic/FrameBuffer.hpp"
#include "gkit/graphic/IndexBuffer.hpp"
#include "gkit/graphic/RenderBuffer.hpp"
#include "gkit/graphic/Shader.hpp"
#include "gkit/graphic/Texture.hpp"
#include "gkit/graphic/VertexArray.hpp"
#include "gkit/graphic/VertexBuffer.hpp"
#include "gkit/graphic/config.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace gkit::graphic {

    /// @brief 后端标识
    enum class Backend : std::uint8_t {
        OpenGL,
        // Vulkan  // 将来
    };

    /**
	 * @brief 抽象渲染设备: 资源工厂 + 渲染命令入口
	 *
	 * 具体后端(OpenGL/Vulkan)继承并实现资源创建与渲染操作。
	 * Renderer 持有 RenderDevice, 通过 create_device 选定后端。
	 */
    class RenderDevice {
    public:
        RenderDevice(const RenderDevice&) = delete;
        auto operator=(const RenderDevice&) -> RenderDevice& = delete;

        RenderDevice()                         = default;
        RenderDevice(RenderDevice&&) noexcept  = default;
        auto operator=(RenderDevice&&) noexcept -> RenderDevice& = default;

        virtual ~RenderDevice() = default;

        // ---- 资源工厂 ----

        virtual auto create_vertex_buffer(const void* data, uint32_t size, bool dynamic = false)
            -> std::unique_ptr<VertexBuffer> = 0;
        virtual auto create_index_buffer(const uint32_t* data, uint32_t count)
            -> std::unique_ptr<IndexBuffer> = 0;
        virtual auto create_shader(const std::string& filepath) -> std::unique_ptr<Shader> = 0;
        /// @brief 纹理创建: 资源模块就绪前为空(见 RHI 设计文档 §4.4/§4.5)
        virtual auto create_texture() -> std::unique_ptr<Texture> = 0;
        virtual auto create_vertex_array() -> std::unique_ptr<VertexArray> = 0;
        virtual auto create_frame_buffer(int width, int height) -> std::unique_ptr<FrameBuffer> = 0;
        virtual auto create_render_buffer(int width, int height) -> std::unique_ptr<RenderBuffer> = 0;
        // 将来: create_storage_buffer() / create_uniform_buffer()

        // ---- 渲染命令入口 ----

        /// @brief 清除当前渲染目标
        virtual auto clear(ClearFlags flags) -> void = 0;

        /// @brief 绘制索引几何体
        virtual auto draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) -> void = 0;

        /// @brief 绘制实例化索引几何体
        virtual auto draw_instance(const VertexArray& va,
                                   const IndexBuffer& ib,
                                   const Shader& shader,
                                   uint32_t instance_count) -> void = 0;
    };

    /// @brief 后端工厂 —— 全工程唯一 switch 点
    auto create_device(Backend backend) -> std::unique_ptr<RenderDevice>;

} // namespace gkit::graphic
