#pragma once

#include "gkit/core/scene/singleton.hpp"
#include "gkit/graphic/opengl/config.hpp"

#include <cstdint>

#include <glad/gl.h>

namespace gkit::graphic::opengl {

    /**
     * @brief OpenGL state manager with dirty flag mechanism
     *
     * Tracks current OpenGL state and only calls GL functions when state actually changes.
     * Uses singleton pattern for global access.
     */
    class StateManager : public core::scene::Singleton<StateManager> {
        friend class core::scene::Singleton<StateManager>;

    private:
        StateManager() = default;

    public:
        /** @brief Depth test state structure */
        struct DepthState {
            bool enabled             = false; // Whether depth test is enabled
            CompareFunc compare_func = CompareFunc::Less; // Depth comparison function
            bool write_mask          = true; // Depth write mask
        };

        /** @brief Blend state structure */
        struct BlendState {
            bool enabled           = false; // Whether blending is enabled
            BlendFunc src_rgb      = BlendFunc::One; // Source RGB blend factor
            BlendFunc dst_rgb      = BlendFunc::Zero; // Destination RGB blend factor
            BlendFunc src_alpha    = BlendFunc::One; // Source alpha blend factor
            BlendFunc dst_alpha    = BlendFunc::Zero; // Destination alpha blend factor
            BlendEquation equation = BlendEquation::Add; // Blend equation
        };

        /** @brief Cull face state structure */
        struct CullFaceState {
            bool enabled         = false; // Whether cull face is enabled
            CullFaceMode mode    = CullFaceMode::Back; // Cull face mode
            FrontFace front_face = FrontFace::CounterClockwise; // Front face winding order
        };

        /** @brief Stencil state structure */
        struct StencilState {
            bool enabled             = false; // Whether stencil test is enabled
            CompareFunc compare_func = CompareFunc::Always; // Stencil comparison function
            uint32_t ref             = 0; // Stencil reference value
            uint32_t read_mask       = 0xFF; // Stencil read mask
            uint32_t write_mask      = 0xFF; // Stencil write mask
            StencilOp fail           = StencilOp::Keep; // Stencil fail operation
            StencilOp z_fail         = StencilOp::Keep; // Stencil depth fail operation
            StencilOp z_pass         = StencilOp::Keep; // Stencil depth pass operation
        };

        /** @brief Enable or disable depth testing
         *  @param enable True to enable depth testing, false to disable
         */
        auto set_depth_test(bool enable) -> void;

        /** @brief Set depth test compare function
         *  @param func The comparison function to use
         */
        auto set_depth_func(CompareFunc func) -> void;

        /** @brief Set depth write mask
         *  @param write True to enable depth writes, false to disable
         */
        auto set_depth_mask(bool write) -> void;

        /** @brief Enable or disable blending
         *  @param enable True to enable blending, false to disable
         */
        auto set_blend(bool enable) -> void;

        /** @brief Set blend factors for RGB and Alpha
         *  @param src_rgb Source RGB blend factor
         *  @param dst_rgb Destination RGB blend factor
         *  @param src_alpha Source alpha blend factor
         *  @param dst_alpha Destination alpha blend factor
         */
        auto set_blend_func(BlendFunc src_rgb, BlendFunc dst_rgb, BlendFunc src_alpha, BlendFunc dst_alpha) -> void;

        /** @brief Set blend equation
         *  @param equation The blend equation to use
         */
        auto set_blend_equation(BlendEquation equation) -> void;

        /** @brief Enable or disable face culling
         *  @param enable True to enable culling, false to disable
         */
        auto set_cull_face(bool enable) -> void;

        /** @brief Set cull face mode
         *  @param mode The cull face mode
         */
        auto set_cull_face_mode(CullFaceMode mode) -> void;

        /** @brief Set front face winding order
         *  @param front_face The front face winding order
         */
        auto set_front_face(FrontFace front_face) -> void;

        /** @brief Enable or disable stencil testing
         *  @param enable True to enable stencil testing, false to disable
         */
        auto set_stencil_test(bool enable) -> void;

        /** @brief Set stencil state
         *  @param func Stencil comparison function
         *  @param ref Stencil reference value
         *  @param mask Stencil read mask
         */
        auto set_stencil(CompareFunc func, uint32_t ref, uint32_t mask) -> void;

        /** @brief Set stencil write mask
         *  @param mask Stencil write mask
         */
        auto set_stencil_mask(uint32_t mask) -> void;

        /** @brief Set stencil operations
         *  @param fail Operation when stencil test fails
         *  @param z_fail Operation when stencil passes but depth fails
         *  @param z_pass Operation when both stencil and depth pass
         */
        auto set_stencil_op(StencilOp fail, StencilOp z_fail, StencilOp z_pass) -> void;

        /** @brief Apply all dirty states to OpenGL */
        auto apply() -> void;

        /** @brief Force apply all states (ignore dirty flags) */
        auto force_apply_all() -> void;

        /** @brief Get current depth state
         *  @return Reference to the current depth state
         */
        [[nodiscard]] auto get_depth_state() const -> const DepthState&;

        /** @brief Get current blend state
         *  @return Reference to the current blend state
         */
        [[nodiscard]] auto get_blend_state() const -> const BlendState&;

        /** @brief Get current cull face state
         *  @return Reference to the current cull face state
         */
        [[nodiscard]] auto get_cull_face_state() const -> const CullFaceState&;

        /** @brief Get current stencil state
         *  @return Reference to the current stencil state
         */
        [[nodiscard]] auto get_stencil_state() const -> const StencilState&;

    private:
        /** @brief Apply depth state if dirty */
        auto apply_depth_state() -> void;

        /** @brief Apply blend state if dirty */
        auto apply_blend_state() -> void;

        /** @brief Apply cull face state if dirty */
        auto apply_cull_face_state() -> void;

        /** @brief Apply stencil state if dirty */
        auto apply_stencil_state() -> void;

        /// @brief Current shadow states
        DepthState depth_state; // Current depth state
        BlendState blend_state; // Current blend state
        CullFaceState cull_face_state; // Current cull face state
        StencilState stencil_state; // Current stencil state

        /// @brief Dirty flags bitmask
        uint8_t dirty_flags = 0; // Bitmask indicating which states need updating

        static constexpr uint8_t DIRTY_DEPTH   = 1 << 0; // Dirty flag for depth state
        static constexpr uint8_t DIRTY_BLEND   = 1 << 1; // Dirty flag for blend state
        static constexpr uint8_t DIRTY_CULL    = 1 << 2; // Dirty flag for cull face state
        static constexpr uint8_t DIRTY_STENCIL = 1 << 3; // Dirty flag for stencil state
    };

} // namespace gkit::graphic::opengl
