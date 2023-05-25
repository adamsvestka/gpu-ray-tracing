#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "common/code_chunks.hpp"
#include "common/load_shaders.hpp"
#include "common/primitives.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <nfd.h>

int main(int argc, char **argv) {
    chunks::setup_glfw();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    int screenWidth = 800, screenHeight = 600;
    GLFWwindow *window = chunks::create_window(screenWidth, screenHeight, "Ray Tracing");

    std::cout << (glGetString(GL_RENDERER)) << std::endl;
    std::cout << (glGetString(GL_VERSION)) << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    chunks::init_dummy_scene();

    GLuint ProgramID = shader::link_program(
        shader::compile_shader(GL_VERTEX_SHADER, shader::load_shader_code("glsl/vertex/main.glsl"), true),
        shader::compile_shader(GL_FRAGMENT_SHADER, shader::load_shader_code("glsl/fragment/main.glsl"), true)
        , true);
    GLuint resolutionID = glGetUniformLocation(ProgramID, "resolution");
    GLuint lightID = glGetUniformLocation(ProgramID, "light");
    GLuint lightStrengthID = glGetUniformLocation(ProgramID, "lightStrength");
    GLuint dataTextureID = glGetUniformLocation(ProgramID, "dataTexture");

    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(glm::vec3(8.0f, 1.0f, 1.0f), 3.0f));
    spheres.push_back(Sphere(glm::vec3(10.0f, 10.0f, 0.0f), 1.0f));

    std::cout << spheres.size() << std::endl;
    std::cout << sizeof(spheres[0]) << std::endl;

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, spheres.size() * sizeof(spheres[0]), spheres.data(), GL_STATIC_DRAW);

    GLuint TBO;
    glGenTextures(1, &TBO);
    glBindTexture(GL_TEXTURE_BUFFER, TBO);
    glBufferData(GL_TEXTURE_BUFFER, spheres.size() * sizeof(spheres[0]), NULL, GL_STATIC_DRAW);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, VBO);
    glUniform1i(dataTextureID, 0);

    glm::vec3 light(0.0f, 4.0f, -3.0f);
    int lightStrength = 100;
    glm::vec2 delta(0.03f, 0.0f);
    float theta = 0.02f;
    glm::mat2 rotate(cos(theta), -sin(theta), sin(theta), cos(theta));

    glUseProgram(ProgramID);
    glUniform2f(resolutionID, screenWidth, screenHeight);

    double lastTime = glfwGetTime();
    int nbFrames = 0;
    double fps = 0.0f;

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    GLint size;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    std::vector<GLfloat> data(size / sizeof(GLfloat));
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, &data[0]);
    for (int i = 0; i < data.size(); i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;

    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) != true) {

        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) {
            fps = nbFrames;
            nbFrames = 0;
            lastTime += 1.0;
        }

        spheres[0].center.y += delta.x;
        spheres[0].center.z += delta.y;

        delta = rotate * delta;

        glUniform3fv(lightID, 1, &light[0]);
        glUniform1i(lightStrengthID, lightStrength);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(spheres[0]), &spheres[0]);
        glTexSubImage1D(GL_TEXTURE_BUFFER, 0, 0, sizeof(spheres[0]), GL_RGBA, GL_FLOAT, &spheres[0]);

        chunks::render_scene();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Another Window");
            ImGui::Text("FPS: %.1f", fps);
            ImGui::SliderFloat("light.x", &light.x, -10.0f, 10.0f);
            ImGui::SliderFloat("light.y", &light.y, -10.0f, 10.0f);
            ImGui::SliderFloat("light.z", &light.z, -10.0f, 10.0f);
            ImGui::SliderInt("lightStrength", &lightStrength, 0, 1000);
            if (ImGui::Button("Open File")) {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog("png,jpg", NULL, &outPath);
                if (result == NFD_OKAY) {
                    std::cout << "Success!" << std::endl;
                    std::cout << outPath << std::endl;
                    free(outPath);
                } else if (result == NFD_CANCEL) {
                    std::cout << "User pressed cancel." << std::endl;
                } else {
                    std::cout << "Error: " << NFD_GetError() << std::endl;
                }
            }
            ImGui::End();
        }

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

    return 0;
}