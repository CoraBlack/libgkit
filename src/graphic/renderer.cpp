#include "gkit/graphic/render/renderer.hpp"

#include "gkit/core/log.hpp"
#include "gkit/graphic/render/render_device.hpp"

namespace gkit::graphic {

    auto Renderer::init(Backend backend) -> void {
        this->device = create_device(backend);
    }

    auto Renderer::clear(ClearFlags flags) -> void {
        this->get_device().clear(flags);
    }

    auto Renderer::draw(RenderObject& obj, const FrameBuffer* target, const Viewport& viewport) -> void {
        // Reject invalid materials: a null shader cannot be bound, and one that
        // failed to compile/link is undefined to render with. Drawing with either
        // would dereference a null/invalid program, so refuse to enqueue instead.
        if (!obj.material.is_valid()) {
            core::Log::Message msg;
            msg.level     = core::Log::LogLevel::Error;
            msg.functions = static_cast<std::uint8_t>(core::Log::LogFunction::Both);
            msg.message   = "Renderer::draw: material has no valid shader; command rejected";
            core::Log::instance().log(std::move(msg));
            return;
        }
        if (obj.instance_count == 0) {
            core::Log::Message msg;
            msg.level     = core::Log::LogLevel::Error;
            msg.functions = static_cast<std::uint8_t>(core::Log::LogFunction::Both);
            msg.message   = "Renderer::draw: instance_count must be >= 1; command rejected";
            core::Log::instance().log(std::move(msg));
            return;
        }

        RenderCommand cmd;
        cmd.object         = &obj; // lazily uploaded on execute
        cmd.target         = target;
        cmd.viewport       = viewport;
        cmd.state          = obj.state; // snapshot: each command carries its own state
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
