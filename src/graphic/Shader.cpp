#include "gkit/graphic/Shader.hpp"

#include "gkit/utils/log.hpp"

#include <fstream>
#include <sstream>

#include <glad/gl.h>

gkit::graphic::Shader::Shader(const std::string& filepath) : file_path(filepath), renderer_id(0) {
    ShaderProgramSource source = parse_shader(filepath);
    this->renderer_id          = create_shader(source.vertex_shader, source.fragment_shader);
}

gkit::graphic::Shader::~Shader() {
    glDeleteProgram(this->renderer_id);
}

gkit::graphic::Shader::Shader(Shader&& other) noexcept :
    renderer_id(other.renderer_id), file_path(std::move(other.file_path)),
    uniform_location_cache(std::move(other.uniform_location_cache)) {
    other.renderer_id = 0;
}

auto gkit::graphic::Shader::operator=(Shader&& other) noexcept -> Shader& {
    if (this != &other) {
        glDeleteProgram(this->renderer_id);
        this->renderer_id            = other.renderer_id;
        this->file_path              = std::move(other.file_path);
        this->uniform_location_cache = std::move(other.uniform_location_cache);
        other.renderer_id            = 0;
    }
    return *this;
}

auto gkit::graphic::Shader::create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
    -> uint32_t {
    uint32_t program = glCreateProgram();
    uint32_t vs      = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    uint32_t fs      = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    if (vs == 0 || fs == 0) {
        glDeleteProgram(program);
        return 0;
    }

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int link_result;
    glGetProgramiv(program, GL_LINK_STATUS, &link_result);
    if (link_result == GL_FALSE) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetProgramInfoLog(program, length, &length, message);
        gkit::utils::Log::Message msg;
        msg.level     = gkit::utils::Log::LogLevel::Error;
        msg.functions = static_cast<std::uint8_t>(gkit::utils::Log::LogFunction::Both);
        msg.message   = "Failed to link shader program: " + std::string(message);
        gkit::utils::Log::instance().log(std::move(msg));
        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return 0;
    }

    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

auto gkit::graphic::Shader::parse_shader(const std::string& file_path) -> ShaderProgramSource {
    gkit::utils::Log::Message path_msg;
    path_msg.level     = gkit::utils::Log::LogLevel::Info;
    path_msg.functions = static_cast<std::uint8_t>(gkit::utils::Log::LogFunction::Both);
    path_msg.message   = "Parsing shader: " + file_path;
    gkit::utils::Log::instance().log(std::move(path_msg));
    std::ifstream stream(file_path);

    /**
     * @brief shader enum type
     */
    enum class ShaderType : std::int8_t {
        NONE     = -1,
        VERTEX   = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else if (type != ShaderType::NONE) {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    if (ss[0].str().empty()) {
        gkit::utils::Log::Message msg;
        msg.level     = gkit::utils::Log::LogLevel::Error;
        msg.functions = static_cast<std::uint8_t>(gkit::utils::Log::LogFunction::Both);
        msg.message   = "No vertex shader found in: " + file_path;
        gkit::utils::Log::instance().log(std::move(msg));
    }
    if (ss[1].str().empty()) {
        gkit::utils::Log::Message msg;
        msg.level     = gkit::utils::Log::LogLevel::Error;
        msg.functions = static_cast<std::uint8_t>(gkit::utils::Log::LogFunction::Both);
        msg.message   = "No fragment shader found in: " + file_path;
        gkit::utils::Log::instance().log(std::move(msg));
    }

    gkit::utils::Log::Message vs_msg;
    vs_msg.level     = gkit::utils::Log::LogLevel::Info;
    vs_msg.functions = static_cast<std::uint8_t>(gkit::utils::Log::LogFunction::Both);
    vs_msg.message   = ss[0].str();
    gkit::utils::Log::instance().log(std::move(vs_msg));

    gkit::utils::Log::Message fs_msg;
    fs_msg.level     = gkit::utils::Log::LogLevel::Info;
    fs_msg.functions = static_cast<std::uint8_t>(gkit::utils::Log::LogFunction::Both);
    fs_msg.message   = ss[1].str();
    gkit::utils::Log::instance().log(std::move(fs_msg));

    return {.vertex_shader = ss[0].str(), .fragment_shader = ss[1].str()};
}

auto gkit::graphic::Shader::compile_shader(uint32_t type, const std::string& source) -> uint32_t {
    uint32_t id     = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        gkit::utils::Log::Message msg;
        msg.level     = gkit::utils::Log::LogLevel::Error;
        msg.functions = static_cast<std::uint8_t>(gkit::utils::Log::LogFunction::Both);
        msg.message   = "Failed to compile " + std::string(type == GL_VERTEX_SHADER ? "vertex" : "fragment") +
                        " shader: " + message;
        gkit::utils::Log::instance().log(std::move(msg));
        glDeleteShader(id);
        return 0;
    }

    return id;
}

auto gkit::graphic::Shader::bind() const -> void {
    glUseProgram(this->renderer_id);
}
auto gkit::graphic::Shader::unbind() const -> void {
    glUseProgram(0);
}

auto gkit::graphic::Shader::set_uniform_1i(const std::string& name, int value) -> void {
    glUniform1i(get_uniform_location(name), value);
}

auto gkit::graphic::Shader::set_uniform_1f(const std::string& name, float value) -> void {
    glUniform1f(get_uniform_location(name), value);
}

auto gkit::graphic::Shader::set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3) -> void {
    glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
}

auto gkit::graphic::Shader::set_uniform_mat_4f(const std::string& name, const float* matrix) -> void {
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, matrix);
}

auto gkit::graphic::Shader::set_uniform_mat_3f(const std::string& name, const float* matrix) -> void {
    glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, matrix);
}

auto gkit::graphic::Shader::set_uniform_vec_4f(const std::string& name, const float* vector4) -> void {
    glUniform4fv(get_uniform_location(name), 1, vector4);
}

auto gkit::graphic::Shader::set_uniform_vec_3f(const std::string& name, const float* vector3) -> void {
    glUniform3fv(get_uniform_location(name), 1, vector3);
}

auto gkit::graphic::Shader::set_uniform_1iv(const std::string& name, const int sz, const int* ind) -> void {
    glUniform1iv(get_uniform_location(name), sz, ind);
}

auto gkit::graphic::Shader::get_uniform_location(const std::string& name) -> int {
    if (this->uniform_location_cache.find(name) != this->uniform_location_cache.end()) {
        return this->uniform_location_cache[name];
    }
    int location = glGetUniformLocation(this->renderer_id, name.c_str());
    if (location == -1) {
        gkit::utils::Log::Message msg;
        msg.level     = gkit::utils::Log::LogLevel::Warning;
        msg.functions = static_cast<std::uint8_t>(gkit::utils::Log::LogFunction::Both);
        msg.message   = "Warning: uniform '" + name + "' doesn't exist!";
        gkit::utils::Log::instance().log(std::move(msg));
    }
    this->uniform_location_cache[name] = location;
    return location;
}
