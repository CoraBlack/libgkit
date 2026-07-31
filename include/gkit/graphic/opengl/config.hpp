#pragma once

#include "gkit/graphic/config.hpp"

#include <cstdint>

#include <glad/gl.h>

namespace gkit::graphic::opengl {

    // ---------------------------------------------------------------------------
    // 后端无关枚举 -> GL 常量映射
    // 前端枚举(见 gkit/graphic/config.hpp)值与 GL 解耦, 集中在此转换为 GL 常量
    // ---------------------------------------------------------------------------

    constexpr auto to_gl_compare_func(CompareFunc func) -> GLenum {
        switch (func) {
        case CompareFunc::Never:
            return GL_NEVER;
        case CompareFunc::Less:
            return GL_LESS;
        case CompareFunc::Equal:
            return GL_EQUAL;
        case CompareFunc::Lequal:
            return GL_LEQUAL;
        case CompareFunc::Greater:
            return GL_GREATER;
        case CompareFunc::Gequal:
            return GL_GEQUAL;
        case CompareFunc::Notequal:
            return GL_NOTEQUAL;
        case CompareFunc::Always:
            return GL_ALWAYS;
        }
        return GL_ALWAYS;
    }

    constexpr auto to_gl_blend_func(BlendFunc func) -> GLenum {
        switch (func) {
        case BlendFunc::Zero:
            return GL_ZERO;
        case BlendFunc::One:
            return GL_ONE;
        case BlendFunc::SrcColor:
            return GL_SRC_COLOR;
        case BlendFunc::OneMinusSrcColor:
            return GL_ONE_MINUS_SRC_COLOR;
        case BlendFunc::DstColor:
            return GL_DST_COLOR;
        case BlendFunc::OneMinusDstColor:
            return GL_ONE_MINUS_DST_COLOR;
        case BlendFunc::SrcAlpha:
            return GL_SRC_ALPHA;
        case BlendFunc::OneMinusSrcAlpha:
            return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFunc::DstAlpha:
            return GL_DST_ALPHA;
        case BlendFunc::OneMinusDstAlpha:
            return GL_ONE_MINUS_DST_ALPHA;
        case BlendFunc::ConstantColor:
            return GL_CONSTANT_COLOR;
        case BlendFunc::OneMinusConstantColor:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case BlendFunc::ConstantAlpha:
            return GL_CONSTANT_ALPHA;
        case BlendFunc::OneMinusConstantAlpha:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        }
        return GL_ONE;
    }

    constexpr auto to_gl_blend_equation(BlendEquation equation) -> GLenum {
        switch (equation) {
        case BlendEquation::Add:
            return GL_FUNC_ADD;
        case BlendEquation::Subtract:
            return GL_FUNC_SUBTRACT;
        case BlendEquation::ReverseSubtract:
            return GL_FUNC_REVERSE_SUBTRACT;
        case BlendEquation::Min:
            return GL_MIN;
        case BlendEquation::Max:
            return GL_MAX;
        }
        return GL_FUNC_ADD;
    }

    constexpr auto to_gl_cull_face_mode(CullFaceMode mode) -> GLenum {
        switch (mode) {
        case CullFaceMode::Front:
            return GL_FRONT;
        case CullFaceMode::Back:
            return GL_BACK;
        case CullFaceMode::FrontAndBack:
            return GL_FRONT_AND_BACK;
        }
        return GL_BACK;
    }

    constexpr auto to_gl_front_face(FrontFace front_face) -> GLenum {
        switch (front_face) {
        case FrontFace::Clockwise:
            return GL_CW;
        case FrontFace::CounterClockwise:
            return GL_CCW;
        }
        return GL_CCW;
    }

    constexpr auto to_gl_stencil_op(StencilOp op) -> GLenum {
        switch (op) {
        case StencilOp::Keep:
            return GL_KEEP;
        case StencilOp::Zero:
            return GL_ZERO;
        case StencilOp::Replace:
            return GL_REPLACE;
        case StencilOp::Incr:
            return GL_INCR;
        case StencilOp::IncrWrap:
            return GL_INCR_WRAP;
        case StencilOp::Decr:
            return GL_DECR;
        case StencilOp::DecrWrap:
            return GL_DECR_WRAP;
        case StencilOp::Invert:
            return GL_INVERT;
        }
        return GL_KEEP;
    }

    constexpr auto to_gl_clear_mask(ClearFlags flags) -> GLbitfield {
        GLbitfield mask = 0;
        const auto bits = static_cast<std::uint8_t>(flags);
        if (bits & static_cast<std::uint8_t>(ClearFlags::Color)) {
            mask |= GL_COLOR_BUFFER_BIT;
        }
        if (bits & static_cast<std::uint8_t>(ClearFlags::Depth)) {
            mask |= GL_DEPTH_BUFFER_BIT;
        }
        if (bits & static_cast<std::uint8_t>(ClearFlags::Stencil)) {
            mask |= GL_STENCIL_BUFFER_BIT;
        }
        return mask;
    }

} // namespace gkit::graphic::opengl

namespace gkit::graphic::opengl::viewport {

    /**
	 * @brief Set viewport with custom position and size
	 *
	 * @param x Left coordinate
	 * @param y Bottom coordinate
	 * @param width Viewport width
	 * @param height Viewport height
	 */
    auto set_viewport(int x, int y, int width, int height) -> void;

    /**
	 * @brief Set viewport with custom size (origin at 0, 0)
	 *
	 * @param width Viewport width
	 * @param height Viewport height
	 */
    auto set_viewport(int width, int height) -> void;

} // namespace gkit::graphic::opengl::viewport
