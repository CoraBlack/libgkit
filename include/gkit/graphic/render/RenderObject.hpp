#pragma once

#include "gkit/graphic/render/RenderCommand.hpp"
#include "gkit/graphic/render/RenderState.hpp"
#include "gkit/graphic/resource/FrameBuffer.hpp"
#include "gkit/graphic/resource/IndexBuffer.hpp"
#include "gkit/graphic/resource/Shader.hpp"
#include "gkit/graphic/resource/Texture.hpp"
#include "gkit/graphic/resource/UniformBuffer.hpp"
#include "gkit/graphic/resource/VertexArray.hpp"

#include <array>
#include <cstdint>

namespace gkit::graphic {

    /**
	 * @brief A reusable draw unit (geometry + material + state)
	 *
	 * Encapsulates everything needed to draw one object. A RenderCommand is
	 * built from it per frame. Fields mirror RenderCommand so the conversion
	 * is a straightforward copy. Shader is non-const (uniforms are mutated
	 * during execution), matching RenderCommand.
	 */
    struct RenderObject {
        const FrameBuffer* target       = nullptr; // Render target (nullptr = screen)
        const VertexArray* vertex_array = nullptr;
        const IndexBuffer* index_buffer = nullptr;
        Shader* shader                  = nullptr; // Non-const: uniforms mutated on execute
        RenderState state; // State snapshot
        UniformData uniforms; // Simple-path per-name uniforms
        UboBlock ubo; // Batch-path UBO reference
        std::array<const Texture*, MAX_TEXTURE_SLOTS> textures = {}; // Texture slots
        uint32_t texture_count                                 = 0; // Number of slots actually used
        uint32_t instance_count                                = 1; // 1 = non-instanced
        bool transparent                                       = false; // Sorting class
        float depth_key                                        = 0.0f; // Depth sort key

        /// @brief Build a draw command from this object
        auto to_command() const -> RenderCommand {
            RenderCommand cmd;
            cmd.target         = this->target;
            cmd.vertex_array   = this->vertex_array;
            cmd.index_buffer   = this->index_buffer;
            cmd.shader         = this->shader;
            cmd.state          = this->state;
            cmd.uniforms       = this->uniforms;
            cmd.ubo            = this->ubo;
            cmd.textures       = this->textures;
            cmd.texture_count  = this->texture_count;
            cmd.instance_count = this->instance_count;
            cmd.transparent    = this->transparent;
            cmd.depth_key      = this->depth_key;
            return cmd;
        }
    };

} // namespace gkit::graphic
