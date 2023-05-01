#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// #include <doctest.h>
#include "common/code_chunks.hpp"
#include "common/load_shaders.hpp"


int main(void) {
    chunks::setup_glfw();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    int width = 1, height = 1;
    chunks::create_window(width, height, "shader test");

    GLuint fb;
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    chunks::init_dummy_scene();

    std::string VertexShaderCode = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex;
        void main() {
            gl_Position = vertex;
        }
    )";

    std::string FragmentShaderInject = R"(
        int test(int a, int b) {
            Sphere s = Sphere_new(vec3(0.0), 1.0);
            return a + b + int(s.radius);
        }
    )";

    GLuint program = shader::link_program(
        shader::compile_shader(GL_VERTEX_SHADER, VertexShaderCode),
        shader::compile_shader(GL_FRAGMENT_SHADER, shader::load_shader_code("glsl/fragment/_testing.glsl", FragmentShaderInject))
    );
    glUseProgram(program);

    chunks::render_scene();

    // GLubyte pixel[4];
    // glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    // printf("%u %u %u %u\n", pixel[0], pixel[1], pixel[2], pixel[3]);
    GLfloat pixel[4];
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, pixel);
    printf("%.0f %.0f %.0f %.0f\n", pixel[0], pixel[1], pixel[2], pixel[3]);

    glDeleteProgram(program);
    glDeleteFramebuffers(1, &fb);

    glfwTerminate();

    return 0;
}