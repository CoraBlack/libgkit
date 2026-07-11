#pragma once
#ifndef CONFIG_H
#define CONFIG_H

namespace gkit::graphic::opengl {

	const unsigned int SCR_WIDTH  = 500;
	const unsigned int SCR_HEIGHT = 500;

	/**
	 * @brief texture pattern
	 */
	enum class TextureType {
		TEXTURE_2D,
		TEXTURE_CUBE_MAP,
		TEXTURE_FRAMEBUFFER
	};

	/**
	 * @brief Compare Functions
	 */
	enum class CompareFunc {
		Never     = 0x0200,  // GL_NEVER
		Less      = 0x0201,  // GL_LESS
		Equal     = 0x0202,  // GL_EQUAL
		Lequal    = 0x0203,  // GL_LEQUAL
		Greater   = 0x0204,  // GL_GREATER
		Gequal    = 0x0205,  // GL_GEQUAL
		Notequal  = 0x0206,  // GL_NOTEQUAL
		Always    = 0x0207,  // GL_ALWAYS
	};

	/**
	 * @brief Blend Functions
	 */
	enum class BlendFunc {
		Zero                  = 0,          // GL_ZERO
		One                   = 1,          // GL_ONE
		SrcColor              = 0x0300,     // GL_SRC_COLOR
		OneMinusSrcColor      = 0x0301,     // GL_ONE_MINUS_SRC_COLOR
		DstColor              = 0x0306,     // GL_DST_COLOR
		OneMinusDstColor      = 0x0307,     // GL_ONE_MINUS_DST_COLOR
		SrcAlpha              = 0x0302,     // GL_SRC_ALPHA
		OneMinusSrcAlpha      = 0x0303,     // GL_ONE_MINUS_SRC_ALPHA
		DstAlpha              = 0x0304,     // GL_DST_ALPHA
		OneMinusDstAlpha      = 0x0305,     // GL_ONE_MINUS_DST_ALPHA
		ConstantColor         = 0x8001,     // GL_CONSTANT_COLOR
		OneMinusConstantColor = 0x8002,     // GL_ONE_MINUS_CONSTANT_COLOR
		ConstantAlpha         = 0x8003,     // GL_CONSTANT_ALPHA
		OneMinusConstantAlpha = 0x8004,     // GL_ONE_MINUS_CONSTANT_ALPHA
	};

	/**
	 * @brief Blend Equations
	 */
	enum class BlendEquation {
		Add              = 0x8006,  // GL_FUNC_ADD
		Subtract         = 0x800A,  // GL_FUNC_SUBTRACT
		ReverseSubtract  = 0x800B,  // GL_FUNC_REVERSE_SUBTRACT
		Min              = 0x8007,  // GL_MIN
		Max              = 0x8008,  // GL_MAX
	};

	/**
	 * @brief Cull Face Modes
	 */
	enum class CullFaceMode {
		Front        = 0x0404,  // GL_FRONT
		Back         = 0x0405,  // GL_BACK
		FrontAndBack = 0x0408,  // GL_FRONT_AND_BACK
	};

	/**
	 * @brief Front Face Winding
	 */
	enum class FrontFace {
		Clockwise           = 0x0900,  // GL_CW
		CounterClockwise    = 0x0901,  // GL_CCW
	};

	/**
	 * @brief Stencil Operations
	 */
	enum class StencilOp {
		Keep     = 0x1E00,  // GL_KEEP
		Zero     = 0,       // GL_ZERO
		Replace  = 0x1E01,  // GL_REPLACE
		Incr     = 0x1E02,  // GL_INCR
		IncrWrap = 0x8507,  // GL_INCR_WRAP
		Decr     = 0x1E03,  // GL_DECR
		DecrWrap = 0x8508,  // GL_DECR_WRAP
		Invert   = 0x150A,  // GL_INVERT
	};

	/**
	 * @brief Clear Options
	 */
	enum class ClearFlags : unsigned int {
		Color      = 0x00004000,  // Clear Color (GL_COLOR_BUFFER_BIT)
		Depth      = 0x00000100,  // Clear Depth (GL_DEPTH_BUFFER_BIT)
		Stencil    = 0x00000400,  // Clear Stencil (GL_STENCIL_BUFFER_BIT)

		ColorDepth = Color | Depth,              // Color + Depth
		All        = Color | Depth | Stencil     // Clear All
	};

	constexpr auto operator|(ClearFlags a, ClearFlags b) noexcept -> ClearFlags {
		return static_cast<ClearFlags>(
			static_cast<unsigned int>(a) | static_cast<unsigned int>(b)
		);
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
#endif