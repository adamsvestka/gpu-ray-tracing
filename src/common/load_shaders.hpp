#pragma once

#include <string>

#include <GL/glew.h>

namespace shader {
    GLuint link_program(GLuint vertex_shader, GLuint fragment_shader, bool throw_errors = false);

    GLuint compile_shader(GLenum shader_type, const std::string &shader_code, bool throw_errors = false);
    
    std::string load_shader_code(const std::string &shader_path, const std::string inject_code = "");
}