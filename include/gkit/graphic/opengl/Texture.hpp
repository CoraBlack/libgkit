#pragma once

#include "gkit/graphic/Texture.hpp"
#include "gkit/graphic/config.hpp"

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief OpenGL 后端纹理对象
 *
 * 继承前端 `graphic::Texture`。
 * 当前为占位实现(自行解码 + 持有 local_buffer),
 * 待资源模块就绪后改为"只持有一个指向资源的指针"(见 RHI 设计文档 §4.4/§4.5)。
 */
namespace gkit::graphic::opengl {

    class Texture final : public graphic::Texture {
    public:
        Texture(const Texture&) = delete;
        auto operator=(const Texture&) -> Texture& = delete;

        Texture(Texture&& other) noexcept;
        auto operator=(Texture&& other) noexcept -> Texture&;

        /**
		 * @brief 构造纹理(占位)
		 * @param path 纹理文件路径(帧缓冲纹理可为空)
		 * @param type 纹理类型
		 * @deprecated 将来由资源模块提供纹理资源
		 */
        [[deprecated("In the future, the texture resources provided by the resource management module will be used. "
                     "Currently, they are merely serving as placeholders.")]]
        explicit Texture(const std::string& path, TextureType type = TextureType::Texture2D);

        ~Texture() override;

        auto bind(unsigned int slot) const -> void override;
        auto unbind() const -> void override;

        [[nodiscard]] auto get_width() const -> int override { return this->width; }
        [[nodiscard]] auto get_height() const -> int override { return this->height; }

        /// @brief 获取 GL 纹理句柄(后端逃生通道)
        [[nodiscard]] inline auto get_renderer_id() const -> uint32_t { return this->renderer_id; }

    private:
        inline static const std::vector<std::string> FACES = {
            "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};

    private:
        uint32_t renderer_id; // GL 纹理句柄
        std::string filepath; // 纹理文件路径
        unsigned char* local_buffer; // 本地像素数据(占位, 将来由资源模块持有)
        int width; // 纹理宽度
        int height; // 纹理高度
        int bpp; // 每像素位数
        TextureType type; // 纹理类型
    };

} // namespace gkit::graphic::opengl
