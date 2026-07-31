#include "gkit/graphic/Renderer.hpp"

#include "gkit/graphic/RenderDevice.hpp"

namespace gkit::graphic {

    auto Renderer::init(Backend backend) -> void {
        this->device = create_device(backend);
    }

    auto Renderer::clear(ClearFlags flags) -> void {
        this->device->clear(flags);
    }

    auto Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) -> void {
        this->device->draw(va, ib, shader);
    }

    auto Renderer::draw_instance(const VertexArray& va,
                                 const IndexBuffer& ib,
                                 const Shader& shader,
                                 uint32_t instance_count) -> void {
        this->device->draw_instance(va, ib, shader, instance_count);
    }

    auto Renderer::get_device() -> RenderDevice& {
        return *this->device;
    }

} // namespace gkit::graphic
