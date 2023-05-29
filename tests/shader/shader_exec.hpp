#pragma once

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "common/code_chunks.hpp"
#include "common/load_shaders.hpp"

#define SHADER_EXEC(type, code) execute_shader_code_##type(#code)

#define CHECK_VEC3_EQUAL(vec1, vec2) \
    CHECK(vec1.x == doctest::Approx(vec2.x)); \
    CHECK(vec1.y == doctest::Approx(vec2.y)); \
    CHECK(vec1.z == doctest::Approx(vec2.z));


extern GLuint fb, tex;
extern GLuint ProgramID;

float execute_shader_code_float(const std::string &code);
glm::vec2 execute_shader_code_vec2(const std::string &code);
glm::vec3 execute_shader_code_vec3(const std::string &code);
glm::vec4 execute_shader_code_vec4(const std::string &code);
