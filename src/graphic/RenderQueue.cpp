#include "gkit/graphic/RenderQueue.hpp"

namespace gkit::graphic {

    namespace {

        /// @brief Bind a texture slot to the shader sampler unit (stub for slot wiring)
        auto bind_textures(const RenderCommand& cmd) -> void {
            for (uint32_t i = 0; i < cmd.texture_count && i < MAX_TEXTURE_SLOTS; ++i) {
                if (cmd.textures[i] != nullptr) {
                    cmd.textures[i]->bind(i);
                }
            }
        }

    } // namespace

    auto RenderQueue::flush(RenderDevice& device) -> void {
        // TODO(Step 5): sort commands (state grouping / front-to-back / back-to-front).
        for (const auto& cmd : this->commands) {
            if (cmd.target != nullptr) {
                cmd.target->bind();
            } else {
                // Default framebuffer (screen). FBO unbind reverts to screen.
            }

            device.apply_state(cmd.state);

            if (cmd.shader != nullptr) {
                cmd.shader->bind();
            }
            bind_textures(cmd);

            // TODO(Step 5): apply uniforms (UniformData/UboBlock) via the shader.

            if (cmd.vertex_array != nullptr && cmd.index_buffer != nullptr && cmd.shader != nullptr) {
                if (cmd.instance_count > 1) {
                    device.draw_instance(*cmd.vertex_array, *cmd.index_buffer, *cmd.shader, cmd.instance_count);
                } else {
                    device.draw(*cmd.vertex_array, *cmd.index_buffer, *cmd.shader);
                }
            }
        }
        this->commands.clear();
    }

} // namespace gkit::graphic
