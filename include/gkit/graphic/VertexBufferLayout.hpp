#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

/**
 * @brief Vertex element data type (backend-agnostic)
 */
enum class VertexElementType : std::uint8_t {
    Float  = 0, // 4-byte float
    Uint32 = 1, // 4-byte unsigned int
    Uint8  = 2, // 1-byte unsigned byte
};

namespace gkit::graphic {

    /**
	 * @brief A single attribute element in a vertex layout
	 */
    struct VertexBufferElement {
        /**
		 * @brief Size in bytes of an element type
		 */
        static constexpr auto get_size_of_type(VertexElementType type) -> uint32_t {
            switch (type) {
            case VertexElementType::Float:
            case VertexElementType::Uint32:
                return 4;
            case VertexElementType::Uint8:
                return 1;
            default:
                assert(false && "Unknown vertex buffer element type");
                return 0;
            }
        }

        VertexElementType type; // element type
        uint32_t count; // number of components
        unsigned char normalized; // whether to normalize
    };

    /**
	 * @brief Vertex buffer layout (frontend definition; backends configure vertex input from it)
	 */
    class VertexBufferLayout {
    public:
        VertexBufferLayout() = default;

        /**
		 * @brief Add an attribute element (unsupported types trigger a compile-time error)
		 * @tparam T data type
		 * @param count number of components
		 */
        template<typename T>
        auto push(uint32_t count) -> void {
            static_assert(sizeof(T) == 0, "Unsupported type for VertexBufferLayout::push");
        }

        /**
		 * @brief Get all elements in this layout (by const reference - zero copy)
		 * @return Const reference to vector of vertex buffer elements
		 */
        [[nodiscard]] inline auto get_elements() const -> const std::vector<VertexBufferElement>& {
            return this->elements;
        }

        /**
		 * @brief Get the stride (bytes between vertices)
		 * @return Stride in bytes
		 */
        [[nodiscard]] inline auto get_stride() const -> uint32_t { return this->stride; }

    private:
        std::vector<VertexBufferElement> elements;
        uint32_t stride = 0;
    };

    // Template specializations

    template<>
    inline auto VertexBufferLayout::push<float>(uint32_t count) -> void {
        assert(count > 0 && "VertexBufferLayout::push count must be greater than 0");
        this->elements.push_back({.type = VertexElementType::Float, .count = count, .normalized = 0});
        this->stride += VertexBufferElement::get_size_of_type(VertexElementType::Float) * count;
    }

    template<>
    inline auto VertexBufferLayout::push<uint32_t>(uint32_t count) -> void {
        assert(count > 0 && "VertexBufferLayout::push count must be greater than 0");
        this->elements.push_back({.type = VertexElementType::Uint32, .count = count, .normalized = 0});
        this->stride += VertexBufferElement::get_size_of_type(VertexElementType::Uint32) * count;
    }

    template<>
    inline auto VertexBufferLayout::push<unsigned char>(uint32_t count) -> void {
        assert(count > 0 && "VertexBufferLayout::push count must be greater than 0");
        this->elements.push_back({.type = VertexElementType::Uint8, .count = count, .normalized = 1});
        this->stride += VertexBufferElement::get_size_of_type(VertexElementType::Uint8) * count;
    }

} // namespace gkit::graphic
