#pragma once

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
	 * @brief Viewport rectangle for a render command
	 *
	 * Each command carries its own viewport so FBO-targeted commands use the
	 * FBO size while screen commands use the window size (GL viewport is global
	 * state, so it must be set per command).
	 */
    struct Viewport {
        int x      = 0; // Left coordinate
        int y      = 0; // Bottom coordinate
        int width  = 0; // Viewport width
        int height = 0; // Viewport height
    };

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
        Shader* shader                  = nullptr; // Non-const: uniforms are mutated during execution
        Viewport viewport; // Viewport to set before drawing (per-target size)
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
