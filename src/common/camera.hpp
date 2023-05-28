#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 rotation;

    Camera() : position(0.0f), rotation(0.0f) {}

    glm::mat4 getTranslationMatrix() const;
    glm::mat4 getRotationMatrix() const;
};
