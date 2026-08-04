#pragma once

#include "gkit/graphic/Buffer.hpp"
#include "gkit/math/matrix3.hpp"
#include "gkit/math/matrix4.hpp"
#include "gkit/math/vector3.hpp"
#include "gkit/math/vector4.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace gkit::graphic {

    /**
	 * @brief A single uniform value (type-erased)
	 */
    using UniformValue = std::variant<int, float, math::Vector3, math::Vector4, math::Matrix3, math::Matrix4>;

    /**
	 * @brief Simple uniform set (逐条赋值)
	 *
	 * 简单路径: 命令携带 name→value 列表, 执行器逐个 set_uniform_*。
	 */
    struct UniformData {
        std::vector<std::pair<std::string, UniformValue>> values;
    };

    /**
	 * @brief UBO block reference (批量上传)
	 *
	 * 批量路径: 命令携带用户参数结构体的引用, 执行器一次上传整个 block。
	 * 持引用不拥有 —— 用户结构体须存活到 flush 结束(生命周期契约)。
	 */
    struct UboBlock {
        const void* data = nullptr; // Pointer to the user struct (e.g. SceneParams)
        size_t size      = 0; // Size of the struct in bytes
        uint32_t binding = 0; // UBO binding point
    };

    /**
	 * @brief Uniform buffer object (UBO) base class (placeholder)
	 *
	 * Buffer that stores uniform data (UBO) for batching shader constants.
	 * Backend (opengl::UniformBuffer) + Device factory not implemented yet.
	 */
    // class UniformBuffer : public Buffer {
    // public:
    //     ~UniformBuffer() override = default;
    // };

} // namespace gkit::graphic
