#include "shader_exec.hpp"

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#include <iostream>

#include <GLFW/glfw3.h>


GLuint fb, tex;

void setup() {
    chunks::setup_glfw();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    int width = 1, height = 1;
    chunks::create_window(width, height, "shader test");

    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    chunks::init_dummy_scene();
}

void teardown() {
    glDeleteFramebuffers(1, &fb);
    glDeleteTextures(1, &tex);

    glfwTerminate();
}

GLuint run_and_get_result(const std::string &binding) {
    static std::string VertexShaderCode = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex;
        void main() {
            gl_Position = vertex;
        }
    )";

    std::string FragmentShaderInject = R"(
        void main() {
            color = )" + binding + R"(;
        }
    )";

    GLuint program;
    try {
        program = shader::link_program(
            shader::compile_shader(GL_VERTEX_SHADER, VertexShaderCode, true),
            shader::compile_shader(GL_FRAGMENT_SHADER, shader::load_shader_code("glsl/fragment/_testing.glsl", FragmentShaderInject), true)
            , true);
    } catch (const std::runtime_error &e) {
        FAIL(std::string(e.what()));
    }
    glUseProgram(program);

    chunks::render_scene();

    return program;
}

float execute_shader_code_float(const std::string &code) {
    GLuint program = run_and_get_result("vec4(" + code + ", 0, 0, 0)");
    float result[4];
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, result);
    glDeleteProgram(program);
    return result[0];
}

glm::vec2 execute_shader_code_vec2(const std::string &code) {
    GLuint program = run_and_get_result("vec4(" + code + ", 0, 0)");
    glm::vec2 result;
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, &result);
    glDeleteProgram(program);
    return result;
}

glm::vec3 execute_shader_code_vec3(const std::string &code) {
    GLuint program = run_and_get_result("vec4(" + code + ", 0)");
    glm::vec3 result;
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, &result);
    glDeleteProgram(program);
    return result;
}

glm::vec4 execute_shader_code_vec4(const std::string &code) {
    GLuint program = run_and_get_result("vec4(" + code + ")");
    glm::vec4 result;
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, &result);
    glDeleteProgram(program);
    return result;
}

int main(int argc, char **argv) {
    doctest::Context context;

    context.applyCommandLine(argc, argv);

    setup();
    int ret = context.run();
    teardown();

    return ret;
}