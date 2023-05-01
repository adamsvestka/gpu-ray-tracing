#include "load_shaders.hpp"

#include <iostream>
#include <vector>
#include <filesystem>

#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_GLSL
#include <stb_include.h>

#define LOG 0

namespace fs = std::filesystem;

GLboolean log_error(GLuint object_id, GLuint status) {
    GLint result = GL_FALSE;
    GLint info_log_length;

    glGetShaderiv(object_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(object_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::vector<char> error_message(info_log_length + 1);

        if (error_message[0] != '\0') {
            glGetShaderInfoLog(object_id, info_log_length, nullptr, &error_message[0]);
            std::cout << &error_message[0] << std::endl;

            return GL_FALSE;
        }
    }
    return GL_TRUE;
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

GLuint shader::compile_shader(GLenum shader_type, const std::string &shader_code) {
    std::cout << "🛠️ Compiling " << (shader_type == GL_VERTEX_SHADER ? "vertex" : (shader_type == GL_FRAGMENT_SHADER ? "fragment" : "unknown")) << " shader : " << std::endl;

    GLuint shader_id = glCreateShader(shader_type);
    const GLchar *shader_code_ptr = shader_code.c_str();
    glShaderSource(shader_id, 1, &shader_code_ptr, nullptr);
    glCompileShader(shader_id);

    if (log_error(shader_id, GL_COMPILE_STATUS)) std::cout << "  ✅ Shader compiled successfully" << std::endl;

    return shader_id;
}

GLuint shader::link_program(GLuint vertex_shader_id, GLuint fragment_shader_id) {
    std::cout << "⚙️ Linking program" << std::endl;

    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    if (log_error(program_id, GL_LINK_STATUS)) std::cout << "  ✅ Program linked successfully" << std::endl;

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}
