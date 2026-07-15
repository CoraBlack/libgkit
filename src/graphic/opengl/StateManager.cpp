#include "gkit/graphic/opengl/StateManager.hpp"

namespace gkit::graphic::opengl {

    auto StateManager::set_depth_test(bool enable) -> void {
        if (this->depth_state.enabled != enable) {
            this->depth_state.enabled = enable;
            this->dirty_flags |= DIRTY_DEPTH;
        }
    }

    auto StateManager::set_depth_func(CompareFunc func) -> void {
        if (this->depth_state.compare_func != func) {
            this->depth_state.compare_func = func;
            this->dirty_flags |= DIRTY_DEPTH;
        }
    }

    auto StateManager::set_depth_mask(bool write) -> void {
        if (this->depth_state.write_mask != write) {
            this->depth_state.write_mask = write;
            this->dirty_flags |= DIRTY_DEPTH;
        }
    }

    auto StateManager::set_blend(bool enable) -> void {
        if (this->blend_state.enabled != enable) {
            this->blend_state.enabled = enable;
            this->dirty_flags |= DIRTY_BLEND;
        }
    }

    auto StateManager::set_blend_func(BlendFunc src_rgb, BlendFunc dst_rgb, BlendFunc src_alpha, BlendFunc dst_alpha)
        -> void {
        if (this->blend_state.src_rgb != src_rgb || this->blend_state.dst_rgb != dst_rgb ||
            this->blend_state.src_alpha != src_alpha || this->blend_state.dst_alpha != dst_alpha) {
            this->blend_state.src_rgb   = src_rgb;
            this->blend_state.dst_rgb   = dst_rgb;
            this->blend_state.src_alpha = src_alpha;
            this->blend_state.dst_alpha = dst_alpha;
            this->dirty_flags |= DIRTY_BLEND;
        }
    }

    auto StateManager::set_blend_equation(BlendEquation equation) -> void {
        if (this->blend_state.equation != equation) {
            this->blend_state.equation = equation;
            this->dirty_flags |= DIRTY_BLEND;
        }
    }

    auto StateManager::set_cull_face(bool enable) -> void {
        if (this->cull_face_state.enabled != enable) {
            this->cull_face_state.enabled = enable;
            this->dirty_flags |= DIRTY_CULL;
        }
    }

    auto StateManager::set_cull_face_mode(CullFaceMode mode) -> void {
        if (this->cull_face_state.mode != mode) {
            this->cull_face_state.mode = mode;
            this->dirty_flags |= DIRTY_CULL;
        }
    }

    auto StateManager::set_front_face(FrontFace front_face) -> void {
        if (this->cull_face_state.front_face != front_face) {
            this->cull_face_state.front_face = front_face;
            this->dirty_flags |= DIRTY_CULL;
        }
    }

    auto StateManager::set_stencil_test(bool enable) -> void {
        if (this->stencil_state.enabled != enable) {
            this->stencil_state.enabled = enable;
            this->dirty_flags |= DIRTY_STENCIL;
        }
    }

    auto StateManager::set_stencil(CompareFunc func, uint32_t ref, uint32_t mask) -> void {
        if (this->stencil_state.compare_func != func || this->stencil_state.ref != ref ||
            this->stencil_state.read_mask != mask) {
            this->stencil_state.compare_func = func;
            this->stencil_state.ref          = ref;
            this->stencil_state.read_mask    = mask;
            this->dirty_flags |= DIRTY_STENCIL;
        }
    }

    auto StateManager::set_stencil_op(StencilOp fail, StencilOp z_fail, StencilOp z_pass) -> void {
        if (this->stencil_state.fail != fail || this->stencil_state.z_fail != z_fail ||
            this->stencil_state.z_pass != z_pass) {
            this->stencil_state.fail   = fail;
            this->stencil_state.z_fail = z_fail;
            this->stencil_state.z_pass = z_pass;
            this->dirty_flags |= DIRTY_STENCIL;
        }
    }

    auto StateManager::set_stencil_mask(uint32_t mask) -> void {
        if (this->stencil_state.write_mask != mask) {
            this->stencil_state.write_mask = mask;
            this->dirty_flags |= DIRTY_STENCIL;
        }
    }

    auto StateManager::apply() -> void {
        if (this->dirty_flags & DIRTY_DEPTH) {
            apply_depth_state();
        }
        if (this->dirty_flags & DIRTY_BLEND) {
            apply_blend_state();
        }
        if (this->dirty_flags & DIRTY_CULL) {
            apply_cull_face_state();
        }
        if (this->dirty_flags & DIRTY_STENCIL) {
            apply_stencil_state();
        }
        this->dirty_flags = 0;
    }

    auto StateManager::force_apply_all() -> void {
        this->dirty_flags = DIRTY_DEPTH | DIRTY_BLEND | DIRTY_CULL | DIRTY_STENCIL;
        apply();
    }

    auto StateManager::apply_depth_state() -> void {
        if (this->depth_state.enabled) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        glDepthFunc(static_cast<GLenum>(this->depth_state.compare_func));
        glDepthMask(this->depth_state.write_mask ? GL_TRUE : GL_FALSE);
    }

    auto StateManager::apply_blend_state() -> void {
        if (this->blend_state.enabled) {
            glEnable(GL_BLEND);
        } else {
            glDisable(GL_BLEND);
        }
        glBlendFuncSeparate(static_cast<GLenum>(this->blend_state.src_rgb),
                            static_cast<GLenum>(this->blend_state.dst_rgb),
                            static_cast<GLenum>(this->blend_state.src_alpha),
                            static_cast<GLenum>(this->blend_state.dst_alpha));
        glBlendEquation(static_cast<GLenum>(this->blend_state.equation));
    }

    auto StateManager::apply_cull_face_state() -> void {
        if (this->cull_face_state.enabled) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
        glCullFace(static_cast<GLenum>(this->cull_face_state.mode));
        glFrontFace(static_cast<GLenum>(this->cull_face_state.front_face));
    }

    auto StateManager::apply_stencil_state() -> void {
        if (this->stencil_state.enabled) {
            glEnable(GL_STENCIL_TEST);
        } else {
            glDisable(GL_STENCIL_TEST);
        }
        glStencilFunc(static_cast<GLenum>(this->stencil_state.compare_func),
                      this->stencil_state.ref,
                      this->stencil_state.read_mask);
        glStencilOp(static_cast<GLenum>(this->stencil_state.fail),
                    static_cast<GLenum>(this->stencil_state.z_fail),
                    static_cast<GLenum>(this->stencil_state.z_pass));
        glStencilMask(this->stencil_state.write_mask);
    }

    auto StateManager::get_depth_state() const -> const DepthState& {
        return this->depth_state;
    }

    auto StateManager::get_blend_state() const -> const BlendState& {
        return this->blend_state;
    }

    auto StateManager::get_cull_face_state() const -> const CullFaceState& {
        return this->cull_face_state;
    }

    auto StateManager::get_stencil_state() const -> const StencilState& {
        return this->stencil_state;
    }

} // namespace gkit::graphic::opengl
