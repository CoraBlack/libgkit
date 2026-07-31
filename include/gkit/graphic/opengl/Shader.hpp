#pragma once

#include "gkit/graphic/Shader.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

/**
 * @brief OpenGL 后端着色器程序
 *
 * 继承前端 `graphic::Shader`, 实现 GL 着色器的加载/编译/链接与 uniform 操作。
 */
namespace gkit::graphic::opengl {

    class Shader final : public graphic::Shader {
    public:
        Shader(const Shader&)                    = delete;
        auto operator=(const Shader&) -> Shader& = delete;

        Shader(Shader&& other) noexcept;
        auto operator=(Shader&& other) noexcept -> Shader&;

        /**
		 * @brief 从文件构造着色器
		 * @param filepath 着色器源码文件路径
		 */
        explicit Shader(const std::string& filepath);

        ~Shader() override;

        auto bind() const -> void override;
        auto unbind() const -> void override;

        auto set_uniform_1i(const std::string& name, int value) -> void override;
        auto set_uniform_1f(const std::string& name, float value) -> void override;
        auto set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3) -> void override;
        auto set_uniform_vec_4f(const std::string& name, const float* vec4) -> void override;
        auto set_uniform_vec_3f(const std::string& name, const float* vec3) -> void override;
        auto set_uniform_mat_4f(const std::string& name, const float* mat4) -> void override;
        auto set_uniform_mat_3f(const std::string& name, const float* mat3) -> void override;
        auto set_uniform_1iv(const std::string& name, int count, const int* values) -> void override;

    private:
        auto parse_shader(const std::string& file_path) -> graphic::ShaderProgramSource;
        auto compile_shader(uint32_t type, const std::string& source) -> uint32_t;
        auto create_shader(const std::string& vertex_shader, const std::string& fragment_shader) -> uint32_t;
        auto get_uniform_location(const std::string& name) -> int;

    private:
        uint32_t renderer_id = 0; // GL 着色器程序句柄
        std::string file_path; // 着色器文件路径
        std::unordered_map<std::string, int> uniform_location_cache; // uniform 位置缓存
    };

} // namespace gkit::graphic::opengl
