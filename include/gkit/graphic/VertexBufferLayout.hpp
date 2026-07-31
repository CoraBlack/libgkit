#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

/**
 * @brief 顶点元素数据类型(与具体后端无关)
 */
enum class VertexElementType : std::uint8_t {
    Float  = 0, // 4 字节浮点
    Uint32 = 1, // 4 字节无符号整型
    Uint8  = 2, // 1 字节无符号字节
};

namespace gkit::graphic {

    /**
	 * @brief 顶点布局中的单个属性元素
	 */
    struct VertexBufferElement {
        /**
		 * @brief 元素类型所占字节数
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

        VertexElementType type; // 元素类型
        uint32_t count; // 分量数量
        unsigned char normalized; // 是否归一化
    };

    /**
	 * @brief 顶点缓冲布局(前端定义, 后端据此配置顶点输入)
	 */
    class VertexBufferLayout {
    public:
        VertexBufferLayout() = default;

        /**
		 * @brief 添加一个属性元素(不支持的类型触发编译期错误)
		 * @tparam T 数据类型
		 * @param count 分量数量
		 */
        template<typename T>
        auto push(uint32_t count) -> void {
            static_assert(sizeof(T) == 0, "Unsupported type for VertexBufferLayout::push");
        }

        /// @brief 所有元素(只读, 零拷贝)
        [[nodiscard]] inline auto get_elements() const -> const std::vector<VertexBufferElement>& {
            return this->elements;
        }

        /// @brief 顶点步长(字节)
        [[nodiscard]] inline auto get_stride() const -> uint32_t { return this->stride; }

    private:
        std::vector<VertexBufferElement> elements;
        uint32_t stride = 0;
    };

    // 模板特化

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
