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
#include "common/shape_buffer.hpp"
#include "common/light_buffer.hpp"
#include "common/camera.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <nfd.h>

int main(int argc, char **argv) {
    chunks::setup_glfw();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    int screenWidth = 1200, screenHeight = 900;
    GLFWwindow *window = chunks::create_window(screenWidth, screenHeight, "Ray Tracing");

    std::cout << (glGetString(GL_RENDERER)) << std::endl;
    std::cout << (glGetString(GL_VERSION)) << std::endl;
    std::cout << "Max objects: " << GL_MAX_TEXTURE_BUFFER_SIZE / sizeof(Shape) << " (" << GL_MAX_TEXTURE_BUFFER_SIZE << " bytes)" << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    chunks::init_dummy_scene();

    GLuint ProgramID = shader::link_program(
        shader::compile_shader(GL_VERTEX_SHADER, shader::load_shader_code("glsl/vertex/main.glsl")),
        shader::compile_shader(GL_FRAGMENT_SHADER, shader::load_shader_code("glsl/fragment/main.glsl"))
    );
    glUseProgram(ProgramID);

    GLuint resolutionID = glGetUniformLocation(ProgramID, "resolution");
    GLuint renderModeID = glGetUniformLocation(ProgramID, "renderMode");
    GLuint cameraTranslationMatrixID = glGetUniformLocation(ProgramID, "cameraTranslationMatrix");
    GLuint cameraRotationMatrixID = glGetUniformLocation(ProgramID, "cameraRotationMatrix");
    GLuint shapesBufferID = glGetUniformLocation(ProgramID, "shapesBuffer");
    GLuint shapesBufferSizeID = glGetUniformLocation(ProgramID, "shapesBufferSize");
    GLuint lightsBufferID = glGetUniformLocation(ProgramID, "lightsBuffer");
    GLuint lightsBufferSizeID = glGetUniformLocation(ProgramID, "lightsBufferSize");


    glUniform2f(resolutionID, screenWidth, screenHeight);


    ShapeBuffer shapes(shapesBufferID, shapesBufferSizeID, 0);
    shapes.addSphere(glm::vec3(8.0f, 1.0f, 1.0f), 3.0f, Material{ glm::vec3(0.0f, 0.0f, 1.0f), 100.0f, 0.5f });
    auto cuboid = shapes.addCuboid(glm::vec3(5.0f, 5.0f, 0.0f), glm::vec3(2.5f), Material{ glm::vec3(1.0f, 0.0f, 0.0f), 500.0f, 0.0f });

    LightBuffer lights(lightsBufferID, lightsBufferSizeID, 1);
    lights.addGlobalLight(0.1f, glm::vec3(1.0f));
    auto point_light = lights.addPointLight(glm::vec3(0.0f, 4.0f, -3.0f), 500, glm::vec3(1.0f));

    shapes.print();
    shapes.printBuffer();
    shapes.printGPUBuffer(ProgramID);

    lights.print();
    lights.printBuffer();
    lights.printGPUBuffer(ProgramID);


    glm::vec3 light = point_light.getPosition();
    int lightStrength = point_light.getIntensity();
    glm::vec2 delta(0.03f, 0.0f);
    float theta = 0.02f;
    glm::mat2 rotate(cos(theta), -sin(theta), sin(theta), cos(theta));
    Camera camera;


    double lastTime = glfwGetTime();
    int nbFrames = 0;
    double fps = 0.0f;
    double xpos, ypos, lastX, lastY;
    const std::vector<std::string> renderModes = { "Ray Tracing", "Normals", "Depth", "Color" };
    int renderMode = 0;

    glfwSwapInterval(1);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) != true) {

        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) {
            fps = nbFrames;
            nbFrames = 0;
            lastTime += 1.0;
        }

        cuboid.move(glm::vec3(0.0f, delta.x, delta.y));
        delta = rotate * delta;

        point_light.setPosition(light);
        point_light.setIntensity(lightStrength);

        float angle = glm::radians(camera.rotation.z);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.position.z -= 0.1f;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.position.z += 0.1f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.position -= glm::vec3(cos(angle), sin(angle), 0.0f) * 0.1f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.position += glm::vec3(cos(angle), sin(angle), 0.0f) * 0.1f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.position -= glm::vec3(sin(angle), -cos(angle), 0.0f) * 0.1f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.position += glm::vec3(sin(angle), -cos(angle), 0.0f) * 0.1f;

        glfwGetCursorPos(window, &xpos, &ypos);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            float delta_x = (float)(lastX - xpos) / 5.0f;
            float delta_y = (float)(lastY - ypos) / 5.0f;
            camera.rotation.z = mod(camera.rotation.z - delta_x, 360.0f);
            camera.rotation.y = clamp(camera.rotation.y + delta_y, -90.0f, 90.0f);
        }
        lastX = xpos;
        lastY = ypos;

        glUniformMatrix4fv(cameraTranslationMatrixID, 1, GL_FALSE, &camera.getTranslationMatrix()[0][0]);
        glUniformMatrix4fv(cameraRotationMatrixID, 1, GL_FALSE, &camera.getRotationMatrix()[0][0]);

        chunks::render_scene();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Another Window");
            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)", camera.position.x, camera.position.y, camera.position.z);
            ImGui::Text("Camera Rotation: (%.1f, %.1f)", camera.rotation.z, camera.rotation.y);
            if (ImGui::BeginCombo("Render Mode", renderModes[renderMode].c_str())) {
                for (int i = 0; i < renderModes.size(); i++) {
                    bool isSelected = (renderMode == i);
                    if (ImGui::Selectable(renderModes[i].c_str(), isSelected)) {
                        renderMode = i;
                        glUniform1i(renderModeID, renderMode);
                    }
                    if (isSelected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
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