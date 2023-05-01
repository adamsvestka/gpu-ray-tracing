#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "common/code_chunks.hpp"
#include "common/load_shaders.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <nfd.h>

int main(void) {
    chunks::setup_glfw();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    int screenWidth = 800, screenHeight = 600;
    GLFWwindow *window = chunks::create_window(screenWidth, screenHeight, "Ray Tracing");

    std::cout << (glGetString(GL_RENDERER)) << std::endl;
    std::cout << (glGetString(GL_VERSION)) << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    chunks::init_dummy_scene();

    GLuint ProgramID = shader::link_program(
        shader::compile_shader(GL_VERTEX_SHADER, shader::load_shader_code("glsl/vertex/main.glsl")),
        shader::compile_shader(GL_FRAGMENT_SHADER, shader::load_shader_code("glsl/fragment/main.glsl"))
    );
    GLuint resolutionID = glGetUniformLocation(ProgramID, "resolution");
    GLuint variableID = glGetUniformLocation(ProgramID, "center");
    GLuint variable2ID = glGetUniformLocation(ProgramID, "center2");
    GLuint lightID = glGetUniformLocation(ProgramID, "light");
    GLuint lightStrengthID = glGetUniformLocation(ProgramID, "lightStrength");

    glm::vec3 center(8.0f, 1.0f, 1.0f);
    glm::vec3 center2(0.0f, 10.0f, 10.0f);
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

    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) != true) {

        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) {
            fps = nbFrames;
            // printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            // printf("FPS: %.1f\n", fps);
            nbFrames = 0;
            lastTime += 1.0;
        }

        center.y += delta.x;
        center.z += delta.y;

        delta = rotate * delta;

        glUniform3fv(variableID, 1, &center[0]);
        glUniform3fv(variable2ID, 1, &center2[0]);
        glUniform3fv(lightID, 1, &light[0]);
        glUniform1i(lightStrengthID, lightStrength);

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
                nfdresult_t result = NFD_OpenDialog("png,jpg;pdf", NULL, &outPath);
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