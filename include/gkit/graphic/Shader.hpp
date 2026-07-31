#pragma once

#include <string>

/**
 * @brief 着色器(前端抽象接口)
 *
 * 加载/编译/绑定着色器程序的统一接口。
 * 具体后端(OpenGL/Vulkan)负责编译、链接与 uniform 设置。
 */
namespace gkit::graphic {

    /**
	 * @brief 着色器源码结构(按阶段拆分)
	 */
    struct ShaderProgramSource {
        std::string vertex_shader; // Vertex shader source code
        std::string fragment_shader; // Fragment shader source code
    };

    class Shader {
    public:
        Shader()                                 = default;
        Shader(const Shader&)                    = delete;
        auto operator=(const Shader&) -> Shader& = delete;

        Shader(Shader&&) noexcept                    = default;
        auto operator=(Shader&&) noexcept -> Shader& = default;

        virtual ~Shader() = default;

        /// @brief 绑定着色器程序到当前后端上下文
        virtual auto bind() const -> void = 0;

        /// @brief 解绑
        virtual auto unbind() const -> void = 0;

        // Uniform setters(由后端实现, 映射到具体 API)

        virtual auto set_uniform_1i(const std::string& name, int value) -> void                              = 0;
        virtual auto set_uniform_1f(const std::string& name, float value) -> void                            = 0;
        virtual auto set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3) -> void = 0;
        virtual auto set_uniform_vec_4f(const std::string& name, const float* vec4) -> void                  = 0;
        virtual auto set_uniform_vec_3f(const std::string& name, const float* vec3) -> void                  = 0;
        virtual auto set_uniform_mat_4f(const std::string& name, const float* mat4) -> void                  = 0;
        virtual auto set_uniform_mat_3f(const std::string& name, const float* mat3) -> void                  = 0;
        virtual auto set_uniform_1iv(const std::string& name, int count, const int* values) -> void          = 0;
    };

} // namespace gkit::graphic
