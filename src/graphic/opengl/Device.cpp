#include "gkit/graphic/opengl/Device.hpp"

#include "gkit/graphic/opengl/FrameBuffer.hpp"
#include "gkit/graphic/opengl/IndexBuffer.hpp"
#include "gkit/graphic/opengl/RenderBuffer.hpp"
#include "gkit/graphic/opengl/Shader.hpp"
#include "gkit/graphic/opengl/Texture.hpp"
#include "gkit/graphic/opengl/VertexArray.hpp"
#include "gkit/graphic/opengl/VertexBuffer.hpp"
#include "gkit/graphic/opengl/config.hpp"

#include <glad/gl.h>

namespace gkit::graphic::opengl {

    auto Device::create_vertex_buffer(const void* data, uint32_t size, bool dynamic)
        -> std::unique_ptr<graphic::VertexBuffer> {
        return std::make_unique<VertexBuffer>(data, size, dynamic);
    }

    auto Device::create_index_buffer(const uint32_t* data, uint32_t count) -> std::unique_ptr<graphic::IndexBuffer> {
        return std::make_unique<IndexBuffer>(data, count);
    }

    auto Device::create_shader(const std::string& filepath) -> std::unique_ptr<graphic::Shader> {
        return std::make_unique<Shader>(filepath);
    }

    auto Device::create_texture() -> std::unique_ptr<graphic::Texture> {
        // 资源模块就绪前, 纹理创建留空(见 RHI 设计文档 §4.4/§4.5)
        return nullptr;
    }

    auto Device::create_vertex_array() -> std::unique_ptr<graphic::VertexArray> {
        return std::make_unique<VertexArray>();
    }

    auto Device::create_frame_buffer(int width, int height) -> std::unique_ptr<graphic::FrameBuffer> {
        return std::make_unique<FrameBuffer>(width, height);
    }

    auto Device::create_render_buffer(int width, int height) -> std::unique_ptr<graphic::RenderBuffer> {
        return std::make_unique<RenderBuffer>(width, height);
    }

    auto Device::clear(ClearFlags flags) -> void {
        auto mask = to_gl_clear_mask(flags);
        glClear(mask);
    }

    auto Device::draw(const graphic::VertexArray& va, const graphic::IndexBuffer& ib, const graphic::Shader& shader)
        -> void {
        shader.bind();
        va.bind();
        ib.bind();
        glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr);
    }

    auto Device::draw_instance(const graphic::VertexArray& va,
                               const graphic::IndexBuffer& ib,
                               const graphic::Shader& shader,
                               uint32_t instance_count) -> void {
        shader.bind();
        va.bind();
        ib.bind();
        glDrawElementsInstanced(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr, instance_count);
    }

} // namespace gkit::graphic::opengl
