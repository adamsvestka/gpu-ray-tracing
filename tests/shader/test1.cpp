#include <doctest.h>
#include "shader_exec.hpp"
#include "common/primitives.hpp"

#include <vector>

TEST_CASE("test return float") {
    float value = SHADER_EXEC(float, 3.14159);
    CHECK(value == 3.14159f);
}

TEST_CASE("test return vec3") {
    glm::vec3 center = SHADER_EXEC(vec3, vec3(1, 2, 3));
    CHECK(center == glm::vec3(1, 2, 3));
}

TEST_CASE("test data texture") {
    static std::string VertexShaderCode = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex;
        void main() {
            gl_Position = vertex;
        }
    )";

    std::string code;
    SUBCASE("vec4 #1") { code = "texelFetch(dataTexture, 0)"; }
    SUBCASE("vec4 #2") { code = "texelFetch(dataTexture, 1)"; }

    std::string FragmentShaderInject = R"(
        void main() {
            color = )" + code + R"(;
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

    GLuint dataTexture = glGetUniformLocation(program, "dataTexture");

    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(glm::vec3(1.2f, 3.4f, 5.6f), 7.8f));

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, spheres.size() * sizeof(spheres[0]), spheres.data(), GL_STATIC_DRAW);

    GLuint TBO;
    glGenTextures(1, &TBO);
    glBindTexture(GL_TEXTURE_BUFFER, TBO);
    glBufferData(GL_TEXTURE_BUFFER, spheres.size() * sizeof(spheres[0]), NULL, GL_STATIC_DRAW);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, VBO);
    glUniform1i(dataTexture, 0);

    glm::vec4 result;
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, &result);
    glDeleteProgram(program);

    CHECK(result.x == 1.2f);
    CHECK(result.y == 3.4f);
    CHECK(result.z == 5.6f);
    CHECK(result.w == 7.8f);
}