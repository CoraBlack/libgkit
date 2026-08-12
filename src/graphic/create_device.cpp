#include "gkit/graphic/render/render_device.hpp"
#include "graphic/backend/opengl/device.hpp"

namespace gkit::graphic {

    auto create_device(Backend backend) -> std::unique_ptr<RenderDevice> {
        switch (backend) {
        case Backend::OpenGL:
            return std::make_unique<opengl::Device>();
        }
        // future: case Backend::Vulkan: return std::make_unique<vulkan::Device>();
        return nullptr;
    }

} // namespace gkit::graphic
