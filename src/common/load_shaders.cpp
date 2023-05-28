#include "load_shaders.hpp"

#include <iostream>
#include <vector>
#include <filesystem>

#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_GLSL
#include <stb_include.h>

#define LOG 0

namespace fs = std::filesystem;

template <GLuint status>
std::optional<std::string> get_status(GLuint object_id) {
    GLint failed = GL_FALSE;
    GLint message_length;

    if constexpr (status == GL_COMPILE_STATUS) {
        glGetShaderiv(object_id, status, &failed);
        glGetShaderiv(object_id, GL_INFO_LOG_LENGTH, &message_length);
    } else if constexpr (status == GL_LINK_STATUS || status == GL_VALIDATE_STATUS) {
        glGetProgramiv(object_id, status, &failed);
        glGetProgramiv(object_id, GL_INFO_LOG_LENGTH, &message_length);
    } else static_assert(status != status, "Invalid status");

    if (failed) return std::nullopt;

    std::vector<char> error_message(message_length + 1);
    glGetShaderInfoLog(object_id, message_length, nullptr, &error_message[0]);

    return std::string(error_message.begin(), error_message.end());
}

std::string shader::load_shader_code(const std::string &shader_path, const std::string inject_code) {
    fs::path absolute_path = fs::current_path() / shader_path;
    fs::path parent_path = absolute_path.parent_path();

    char error[256];
    char *code = stb_include_file(const_cast<char *>(absolute_path.c_str()), const_cast<char *>(inject_code.c_str()), const_cast<char *>(parent_path.c_str()), error);
    if (code == nullptr) {
        std::cout << error << std::endl;
        return "";
    }

    std::string shader_code(code);
    free(code);

    if (LOG) std::cout << shader_code << std::endl;

    return shader_code;
}

GLuint shader::compile_shader(GLenum shader_type, const std::string &shader_code, bool throw_errors) {
    if (!throw_errors) std::cout << "🛠️ Compiling " << (shader_type == GL_VERTEX_SHADER ? "vertex" : (shader_type == GL_FRAGMENT_SHADER ? "fragment" : "unknown")) << " shader : " << std::endl;

    GLuint shader_id = glCreateShader(shader_type);
    const GLchar *shader_code_ptr = shader_code.c_str();
    glShaderSource(shader_id, 1, &shader_code_ptr, nullptr);
    glCompileShader(shader_id);

    auto status = get_status<GL_COMPILE_STATUS>(shader_id);
    if (!throw_errors) {
        if (!status.has_value()) std::cout << "  ✅ Shader compiled successfully" << std::endl;
        else {
            std::cout << "  ❌ Shader compilation failed" << std::endl;
            std::cout << status.value() << std::endl;
            return 0;
        }
    } else if (status.has_value()) throw std::runtime_error(status.value());

    return shader_id;
}

GLuint shader::link_program(GLuint vertex_shader_id, GLuint fragment_shader_id, bool throw_errors) {
    if (!throw_errors) std::cout << "⚙️ Linking program" << std::endl;

    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    auto status = get_status<GL_LINK_STATUS>(program_id);
    if (!throw_errors) {
        if (!status.has_value()) std::cout << "  ✅ Program linked successfully" << std::endl;
        else {
            std::cout << "  ❌ Program linking failed" << std::endl;
            std::cout << status.value() << std::endl;
            return 0;
        }
    } else if (status.has_value()) throw std::runtime_error(status.value());

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}
