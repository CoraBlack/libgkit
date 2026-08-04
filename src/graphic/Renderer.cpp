#include "gkit/graphic/render/Renderer.hpp"

#include "gkit/graphic/render/RenderDevice.hpp"

namespace gkit::graphic {

    auto Renderer::init(Backend backend) -> void {
        this->device = create_device(backend);
    }

    auto Renderer::clear(ClearFlags flags) -> void {
        this->get_device().clear(flags);
    }

    auto Renderer::draw(RenderObject& obj, const FrameBuffer* target, const Viewport& viewport) -> void {
        RenderCommand cmd;
        cmd.object         = &obj; // lazily uploaded on execute
        cmd.target         = target;
        cmd.viewport       = viewport;
        cmd.instance_count = obj.instance_count;
        cmd.transparent    = obj.transparent;
        cmd.depth_key      = obj.depth_key;
        cmd.clear          = obj.clear;
        cmd.clear_flags    = obj.clear_flags;
        this->queue.submit(cmd);
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
