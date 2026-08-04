#include "gkit/graphic/render/Renderer.hpp"

#include "gkit/graphic/render/RenderDevice.hpp"

namespace gkit::graphic {

    auto Renderer::init(Backend backend) -> void {
        this->device = create_device(backend);
    }

    auto Renderer::clear(ClearFlags flags) -> void {
        this->get_device().clear(flags);
    }

    auto Renderer::draw(const RenderObject& obj) -> void {
        this->queue.submit(obj.to_command());
    }

    auto Renderer::flush() -> void {
        this->queue.flush(this->get_device());
    }

    auto Renderer::get_device() -> RenderDevice& {
        if (this->device == nullptr) {
            this->device = create_device(Backend::OpenGL);
        }
        return *this->device;
    }

} // namespace gkit::graphic
