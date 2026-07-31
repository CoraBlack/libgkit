#pragma once

#include "gkit/graphic/Buffer.hpp"

#include <cstdint>

namespace gkit::graphic {

    /**
	 * @brief Index data buffer (frontend abstract interface)
	 *
	 * Stores draw-order index data for efficient indexed geometry rendering.
	 */
    class IndexBuffer : public Buffer {
    public:
        ~IndexBuffer() override = default;

        /// @brief Number of indices
        [[nodiscard]] virtual auto get_count() const -> uint32_t = 0;
    };

} // namespace gkit::graphic
