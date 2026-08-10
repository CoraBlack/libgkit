#pragma once

#include "gkit/graphic/config.hpp"
#include "gkit/graphic/resource/Shader.hpp"
#include "gkit/graphic/resource/Texture.hpp"
#include "gkit/graphic/resource/UniformBuffer.hpp"

#include <array>
#include <cassert>
#include <cstdint>

namespace gkit::graphic {

    /**
	 * @brief A reusable material (shader + texture slots + uniforms)
	 *
	 * Shader and textures are held by pointer (referenced, not owned); their
	 * lifetime is managed by the resource system. A Material can be shared by
	 * multiple RenderObjects.
	 */
    class Material {
    public:
        Shader* shader = nullptr; // Shader (pointer reference, not owned)
        UniformData uniforms; // Simple-path per-name uniforms
        UboBlock ubo; // Batch-path UBO reference

        /**
		 * @brief Append a texture to the next free slot (slot is managed internally,
		 *        so callers can't create gaps or overflow the slot array)
		 * @return false if all slots are used (asserts in debug builds)
		 */
        auto set_texture(const Texture& texture) -> bool {
            if (this->texture_count >= MAX_TEXTURE_SLOTS) {
                assert(false && "Material::set_texture: texture slots exhausted");
                return false;
            }
            this->textures[this->texture_count] = &texture;
            ++this->texture_count;
            return true;
        }

        /**
		 * @brief Number of textures appended so far
		 */
        [[nodiscard]] auto get_texture_count() const -> uint32_t { return this->texture_count; }

        /**
		 * @brief Texture bound at a slot (nullptr if the slot is unused or out of range)
		 * @param slot texture slot index
		 * @return the texture at that slot, or nullptr
		 */
        [[nodiscard]] auto get_texture(uint32_t slot) const -> const Texture* {
            return slot < this->texture_count ? this->textures[slot] : nullptr;
        }

        /**
		 * @brief Whether the material references a valid shader (null shader is invalid)
		 */
        [[nodiscard]] auto is_valid() const -> bool { return this->shader != nullptr && this->shader->is_valid(); }

    private:
        std::array<const Texture*, MAX_TEXTURE_SLOTS> textures = {}; // Texture slots (pointer reference)
        uint32_t texture_count                                 = 0; // Number of textures appended so far
    };

} // namespace gkit::graphic
