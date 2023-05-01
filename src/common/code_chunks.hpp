#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace chunks {
    void setup_glfw();

    GLFWwindow *create_window(int &width, int &height, const char *title);

    void init_dummy_scene();

    void render_scene();
}