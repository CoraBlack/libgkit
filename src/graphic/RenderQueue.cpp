#include "gkit/graphic/render/RenderQueue.hpp"

#include <algorithm>
#include <variant>

namespace gkit::graphic {

    namespace {

        /// @brief Bind a texture slot to the shader sampler unit
        auto bind_textures(const RenderCommand& cmd) -> void {
            for (uint32_t i = 0; i < cmd.texture_count && i < MAX_TEXTURE_SLOTS; ++i) {
                if (cmd.textures[i] != nullptr) {
                    cmd.textures[i]->bind(i);
                }
            }
        }

        /// @brief Apply a single uniform value through the shader's set_uniform_*
        auto apply_uniform_value(Shader& shader, const std::string& name, const UniformValue& value) -> void {
            std::visit(
                [&](const auto& v) {
                    using T = std::decay_t<decltype(v)>;
                    if constexpr (std::is_same_v<T, int>) {
                        shader.set_uniform_1i(name, v);
                    } else if constexpr (std::is_same_v<T, float>) {
                        shader.set_uniform_1f(name, v);
                    } else if constexpr (std::is_same_v<T, math::Vector4>) {
                        shader.set_uniform_vec_4f(name, v);
                    } else if constexpr (std::is_same_v<T, math::Vector3>) {
                        shader.set_uniform_vec_3f(name, v);
                    } else if constexpr (std::is_same_v<T, math::Matrix4>) {
                        shader.set_uniform_mat_4f(name, v);
                    } else if constexpr (std::is_same_v<T, math::Matrix3>) {
                        shader.set_uniform_mat_3f(name, v);
                    }
                },
                value);
        }

        /// @brief Apply simple-path uniforms (per-name list)
        auto apply_uniforms(const RenderCommand& cmd) -> void {
            if (cmd.shader == nullptr) {
                return;
            }
            for (const auto& [name, value] : cmd.uniforms.values) {
                apply_uniform_value(*cmd.shader, name, value);
            }
            // TODO(Step 6+/UBO): upload cmd.ubo via a UniformBuffer backend once implemented.
        }

        /// @brief Sort comparator: framebuffer commands first, then by state/transparency
        auto sort_key(const RenderCommand& cmd) -> uint64_t {
            // Render targets (FBO) must be drawn before screen commands, otherwise
            // post-processing cannot sample the FBO attachment. So target=null (screen)
            // sorts after any non-null target. Then group by state (reduce switches).
            // Within the same target, opaque front-to-back / transparent back-to-front.
            const uint64_t target_rank = (cmd.target != nullptr) ? 0 : 1; // FBO before screen
            return (target_rank << 56) | (static_cast<uint64_t>(cmd.state.blend.enabled) << 48) |
                   (static_cast<uint64_t>(cmd.transparent) << 40);
        }

    } // namespace

    auto RenderQueue::flush(RenderDevice& device) -> void {
        // Sort: opaque front-to-back, transparent back-to-front; group state/shader.
        std::stable_sort(
            this->commands.begin(), this->commands.end(), [](const RenderCommand& a, const RenderCommand& b) {
                if (sort_key(a) != sort_key(b)) {
                    return sort_key(a) < sort_key(b);
                }
                // Within the same transparency class: opaque nearer-first, transparent farther-first.
                const float key_a = a.transparent ? -a.depth_key : a.depth_key;
                const float key_b = b.transparent ? -b.depth_key : b.depth_key;
                return key_a < key_b;
            });

        const FrameBuffer* last_target = nullptr;
        for (const auto& cmd : this->commands) {
            // Switch render target: unbind the previous FBO (reverting to screen)
            // before binding a different target. target=null means screen, reached
            // by unbinding the previous FBO.
            if (cmd.target != last_target) {
                if (last_target != nullptr) {
                    last_target->unbind();
                }
                last_target = cmd.target;
                if (cmd.target != nullptr) {
                    cmd.target->bind();
                }
            }

            // GL viewport is global state; each command sets its own per-target viewport.
            device.set_viewport(cmd.viewport);

            device.apply_state(cmd.state);

            if (cmd.shader != nullptr) {
                cmd.shader->bind();
            }
            bind_textures(cmd);
            apply_uniforms(cmd);

            if (cmd.vertex_array != nullptr && cmd.index_buffer != nullptr && cmd.shader != nullptr) {
                if (cmd.instance_count > 1) {
                    device.draw_instance(*cmd.vertex_array, *cmd.index_buffer, *cmd.shader, cmd.instance_count);
                } else {
                    device.draw(*cmd.vertex_array, *cmd.index_buffer, *cmd.shader);
                }
            }
        }

        // End of frame: leave the default framebuffer bound (screen).
        if (last_target != nullptr) {
            last_target->unbind();
        }
        this->commands.clear();
    }

} // namespace gkit::graphic
