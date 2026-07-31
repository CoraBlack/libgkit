#include "gkit/graphic/Renderer.hpp"

#include "gkit/graphic/Shader.hpp"
#include "gkit/graphic/config.hpp"
#include "gkit/graphic/opengl/config.hpp"

#include <glad/gl.h>

namespace gkit::graphic {

    auto Renderer::clear(ClearFlags flags) const -> void {
        auto mask = opengl::to_gl_clear_mask(flags);
        glClear(mask);
    }

    auto Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const -> void {
        shader.bind();
        va.bind();
        ib.bind();
        glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr);
    }

    auto Renderer::draw_instance(const VertexArray& va,
                                 const IndexBuffer& ib,
                                 const Shader& shader,
                                 uint32_t instance_count) const -> void {
        shader.bind();
        va.bind();
        ib.bind();
        glDrawElementsInstanced(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr, instance_count);
    }

} // namespace gkit::graphic
