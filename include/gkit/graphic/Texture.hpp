#pragma once

namespace gkit::graphic {

    /**
	 * @brief 纹理(前端抽象接口)
	 *
	 * CPU 资源(像素数据, 由资源模块持有) ↔ GPU 纹理对象(由后端持有) 的薄绑定。
	 * 不拥有像素数据、不负责解码。
	 *
	 * TODO(将来): 资源模块就绪后, 构造/工厂入参改为资源句柄
	 *             (shared_ptr<resource::Texture> 或 GUID), 见 RHI 设计文档 §4.4/§4.5。
	 *             当前 `opengl::Texture` 为占位实现。
	 */
    class Texture {
    public:
        Texture()                        = default;
        Texture(const Texture&)          = delete;
        auto operator=(const Texture&) -> Texture& = delete;

        Texture(Texture&&) noexcept            = default;
        auto operator=(Texture&&) noexcept -> Texture& = default;

        virtual ~Texture() = default;

        /// @brief 绑定到指定槽位
        virtual auto bind(unsigned int slot) const -> void = 0;

        /// @brief 解绑
        virtual auto unbind() const -> void = 0;

        /// @brief 纹理宽度(像素)
        [[nodiscard]] virtual auto get_width() const -> int = 0;

        /// @brief 纹理高度(像素)
        [[nodiscard]] virtual auto get_height() const -> int = 0;
    };

} // namespace gkit::graphic
