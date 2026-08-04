#pragma once

#include "gkit/graphic/FrameBuffer.hpp"
#include "gkit/graphic/IndexBuffer.hpp"
#include "gkit/graphic/RenderState.hpp"
#include "gkit/graphic/Shader.hpp"
#include "gkit/graphic/Texture.hpp"
#include "gkit/graphic/UniformBuffer.hpp"
#include "gkit/graphic/VertexArray.hpp"

#include <array>
#include <cstdint>

namespace gkit::graphic {

    /**
	 * @brief Engine-declared texture slot limit (fixed conservative value)
	 *
	 * Most shaders fit within 8 slots; not chasing hardware limits
	 * (GL_MAX_TEXTURE_IMAGE_UNITS varies by GPU, usually >= 32).
	 * Start up may assert the hardware supports at least this many.
	 */
    static constexpr uint32_t MAX_TEXTURE_SLOTS = 8;

    /**
	 * @brief A single draw command carrying its complete render state
	 *
	 * Value type; references (not owns) resources. Sorting keys and state are
	 * self-contained so the queue can reorder without global mutable state.
	 */
    struct RenderCommand {
        const FrameBuffer* target       = nullptr; // Render target (nullptr = screen)
        const VertexArray* vertex_array = nullptr;
        const IndexBuffer* index_buffer = nullptr;
        const Shader* shader            = nullptr;
        RenderState state; // State snapshot (sorting key)
        UniformData uniforms; // Simple-path per-name uniforms (see design §5.1)
        UboBlock ubo; // Batch-path UBO reference (see design §5.2)
        std::array<const Texture*, MAX_TEXTURE_SLOTS> textures = {}; // Texture slots (slot ↔ shader sampler unit)
        uint32_t texture_count                                 = 0; // Number of slots actually used
        uint32_t instance_count                                = 1; // 1 = non-instanced
        bool transparent = false; // Sort front-to-back (opaque) or back-to-front (transparent)
        float depth_key  = 0.0f; // Depth sort key (filled by upper layer)
    };

} // namespace gkit::graphic
