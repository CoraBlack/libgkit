#pragma once

#include "gkit/core/templates/singleton.hpp"
#include "gkit/graphic/RenderDevice.hpp"
#include "gkit/graphic/config.hpp"

#include <cstdint>
#include <memory>

/**
 * @brief Renderer class providing public rendering interface
 *
 * The Renderer provides a unified interface for rendering operations.
 * Uses singleton pattern for global access.
 * Holds a RenderDevice selected via init(Backend).
 */
namespace gkit::graphic {

    class Renderer : public core::templates::Singleton<Renderer> {
        friend class core::templates::Singleton<Renderer>;

    private:
        Renderer() = default;

    public:
        /**
		 * @brief 选择渲染后端并创建设备(默认 OpenGL)
		 * @param backend 后端标识
		 */
        auto init(Backend backend = Backend::OpenGL) -> void;

        /**
		 * @brief Clear the current framebuffer
		 *
		 * @param flags Bitmask specifying which buffers to clear (e.g., ClearFlags::Color | ClearFlags::Depth)
		 *              Defaults to ClearFlags::All (clears all buffers)
		 */
        auto clear(ClearFlags flags = ClearFlags::All) -> void;

        /**
		 * @brief Draw indexed geometry
		 * @param va Vertex array containing vertex data
		 * @param ib Index buffer containing indices
		 * @param shader Shader program to use for rendering
		 */
        auto draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) -> void;

        /**
		 * @brief Draw multiple instances of indexed geometry
		 * @param va Vertex array containing vertex data
		 * @param ib Index buffer containing indices
		 * @param shader Shader program to use for rendering
		 * @param instance_count Number of instances to draw
		 */
        auto draw_instance(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, uint32_t instance_count)
            -> void;

        /// @brief 访问当前渲染设备
        [[nodiscard]] auto get_device() -> RenderDevice&;

    private:
        std::unique_ptr<RenderDevice> device;
    };

} // namespace gkit::graphic
