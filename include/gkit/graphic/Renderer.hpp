#pragma once

#include "gkit/core/templates/singleton.hpp"
#include "gkit/graphic/RenderDevice.hpp"
#include "gkit/graphic/RenderObject.hpp"
#include "gkit/graphic/RenderQueue.hpp"
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
		 * @brief Select the render backend and create the device (defaults to OpenGL)
		 * @param backend backend identifier
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
		 * @brief Enqueue an indexed draw
		 * @param va Vertex array containing vertex data
		 * @param ib Index buffer containing indices
		 * @param shader Shader program to use for rendering
		 * @note Enqueued into the render queue; executed on flush(). Shader is
		 *       non-const because uniforms are mutated during execution.
		 */
        auto draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader) -> void;

        /**
		 * @brief Enqueue a draw from a reusable render object
		 * @param obj Render object (geometry + material + state)
		 */
        auto draw(const RenderObject& obj) -> void;

        /**
		 * @brief Enqueue an instanced indexed draw
		 * @param va Vertex array containing vertex data
		 * @param ib Index buffer containing indices
		 * @param shader Shader program to use for rendering
		 * @param instance_count Number of instances to draw
		 */
        auto draw_instance(const VertexArray& va, const IndexBuffer& ib, Shader& shader, uint32_t instance_count)
            -> void;

        /**
		 * @brief Execute the queued render commands (sort + apply state + draw)
		 */
        auto flush() -> void;

        /**
		 * @brief Access the current render device
		 */
        [[nodiscard]] auto get_device() -> RenderDevice&;

    private:
        std::unique_ptr<RenderDevice> device;
        RenderQueue queue;
    };

} // namespace gkit::graphic
