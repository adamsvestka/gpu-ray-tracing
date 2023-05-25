#pragma once

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "common/code_chunks.hpp"
#include "common/load_shaders.hpp"

#define SHADER_EXEC(type, code) execute_shader_code_##type(#code)


extern GLuint fb, tex;

float execute_shader_code_float(const std::string &code);
glm::vec2 execute_shader_code_vec2(const std::string &code);
glm::vec3 execute_shader_code_vec3(const std::string &code);
glm::vec4 execute_shader_code_vec4(const std::string &code);
