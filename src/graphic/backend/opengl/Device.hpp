#pragma once

#include "gkit/graphic/RenderDevice.hpp"

#include <memory>
#include <string>

/**
 * @brief OpenGL backend render device
 *
 * Inherits frontend `graphic::RenderDevice`; implements the resource factory
 * and GL render commands.
 */
namespace gkit::graphic::opengl {

    class Device final : public graphic::RenderDevice {
    public:
        Device()           = default;
        ~Device() override = default;

        auto create_vertex_buffer(const void* data, uint32_t size, bool dynamic)
            -> std::unique_ptr<graphic::VertexBuffer> override;
        auto create_index_buffer(const uint32_t* data, uint32_t count)
            -> std::unique_ptr<graphic::IndexBuffer> override;
        auto create_shader(const std::string& filepath) -> std::unique_ptr<graphic::Shader> override;
        auto create_texture() -> std::unique_ptr<graphic::Texture> override;
        auto create_vertex_array() -> std::unique_ptr<graphic::VertexArray> override;
        auto create_frame_buffer(int width, int height) -> std::unique_ptr<graphic::FrameBuffer> override;
        auto create_render_buffer(int width, int height) -> std::unique_ptr<graphic::RenderBuffer> override;

        auto clear(ClearFlags flags) -> void override;
        auto draw(const graphic::VertexArray& va, const graphic::IndexBuffer& ib, const graphic::Shader& shader)
            -> void override;
        auto draw_instance(const graphic::VertexArray& va,
                           const graphic::IndexBuffer& ib,
                           const graphic::Shader& shader,
                           uint32_t instance_count) -> void override;
    };

} // namespace gkit::graphic::opengl
