#pragma once

#include "gkit/graphic/Buffer.hpp"

namespace gkit::graphic {

    /**
	 * @brief Vertex data buffer (frontend abstract interface)
	 *
	 * Buffer that stores vertex data (position/color/uv, etc.) on the GPU.
	 */
    class VertexBuffer : public Buffer {
    public:
        ~VertexBuffer() override = default;

        /**
		 * @brief Whether this is a dynamic buffer (updated frequently; backend maps to GL_DYNAMIC_DRAW etc.)
		 */
        [[nodiscard]] virtual auto is_dynamic() const -> bool = 0;
    };

} // namespace gkit::graphic
