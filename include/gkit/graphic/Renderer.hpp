#pragma once

#include "gkit/core/templates/singleton.hpp"
#include "gkit/graphic/Shader.hpp"
#include "gkit/graphic/config.hpp"
#include "gkit/graphic/IndexBuffer.hpp"
#include "gkit/graphic/VertexArray.hpp"

#include <cstdint>

/**
 * @brief Renderer class providing public rendering interface
 *
 * The Renderer provides a unified interface for rendering operations.
 * Uses singleton pattern for global access.
 */
namespace gkit::graphic {

    class Renderer : public core::templates::Singleton<Renderer> {
        friend class core::templates::Singleton<Renderer>;

    private:
        Renderer() = default;

    public:
        /**
		 * @brief Clear the current framebuffer
		 *
		 * @param flags Bitmask specifying which buffers to clear (e.g., ClearFlags::Color | ClearFlags::Depth)
		 *              Defaults to ClearFlags::All (clears all buffers)
		 */
        auto clear(ClearFlags flags = ClearFlags::All) const -> void;

        /**
		 * @brief Draw indexed geometry
		 * @param va Vertex array containing vertex data
		 * @param ib Index buffer containing indices
		 * @param shader Shader program to use for rendering
		 */
        auto draw(const gkit::graphic::VertexArray& va,
                  const gkit::graphic::IndexBuffer& ib,
                  const gkit::graphic::Shader& shader) const -> void;

        /**
		 * @brief Draw multiple instances of indexed geometry
		 * @param va Vertex array containing vertex data
		 * @param ib Index buffer containing indices
		 * @param shader Shader program to use for rendering
		 * @param instance_count Number of instances to draw
		 */
        auto draw_instance(const gkit::graphic::VertexArray& va,
                           const gkit::graphic::IndexBuffer& ib,
                           const gkit::graphic::Shader& shader,
                           uint32_t instance_count) const -> void;
    };

} // namespace gkit::graphic