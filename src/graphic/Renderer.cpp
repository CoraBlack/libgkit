#include "gkit/graphic/Renderer.hpp"

#include "gkit/graphic/RenderDevice.hpp"

namespace gkit::graphic {

    auto Renderer::init(Backend backend) -> void {
        this->device = create_device(backend);
    }

    auto Renderer::clear(ClearFlags flags) -> void {
        this->get_device().clear(flags);
    }

    auto Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) -> void {
        this->get_device().draw(va, ib, shader);
    }

    auto Renderer::draw_instance(const VertexArray& va,
                                 const IndexBuffer& ib,
                                 const Shader& shader,
                                 uint32_t instance_count) -> void {
        this->get_device().draw_instance(va, ib, shader, instance_count);
    }

    auto Renderer::get_device() -> RenderDevice& {
        // Lazily create the default device so callers don't have to ensure
        // init() was called before get_device().
        if (this->device == nullptr) {
            this->device = create_device(Backend::OpenGL);
        }
        return *this->device;
    }

} // namespace gkit::graphic
