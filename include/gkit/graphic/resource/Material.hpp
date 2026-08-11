#pragma once

#include "gkit/graphic/config.hpp"
#include "gkit/graphic/resource/Shader.hpp"
#include "gkit/graphic/resource/Texture.hpp"
#include "gkit/graphic/resource/UniformBuffer.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <string>
#include <utility>

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
        /**
		 * @brief Set the shader used by this material (pointer reference, not owned)
		 */
        auto set_shader(Shader* shader) -> void { this->shader = shader; }

        /**
		 * @brief The bound shader (nullptr if none)
		 */
        [[nodiscard]] auto get_shader() const -> Shader* { return this->shader; }

        /**
		 * @brief Set a uniform value by name (simple path: name → value list)
		 * @param name uniform name as declared in the shader
		 * @param value type-erased uniform value (int/float/Vector/Matrix)
		 */
        auto set_uniform(const std::string& name, const UniformValue& value) -> void {
            for (auto& [existing, v] : this->uniforms.values) {
                if (existing == name) {
                    v = value;
                    return;
                }
            }
            this->uniforms.values.emplace_back(name, value);
        }

        /**
		 * @brief The uniform list (simple path), read-only
		 */
        [[nodiscard]] auto get_uniforms() const -> const UniformData& { return this->uniforms; }

        /**
		 * @brief Set the UBO block reference (batch path)
		 */
        auto set_ubo(const UboBlock& ubo) -> void { this->ubo = ubo; }

        /**
		 * @brief The UBO block reference (batch path), read-only
		 */
        [[nodiscard]] auto get_ubo() const -> const UboBlock& { return this->ubo; }

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
        Shader* shader = nullptr; // Shader (pointer reference, not owned)
        UniformData uniforms; // Simple-path per-name uniforms
        UboBlock ubo; // Batch-path UBO reference
        std::array<const Texture*, MAX_TEXTURE_SLOTS> textures = {}; // Texture slots (pointer reference)
        uint32_t texture_count                                 = 0; // Number of textures appended so far
    };

} // namespace gkit::graphic
