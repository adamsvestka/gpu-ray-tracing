#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getTranslationMatrix() const {
    return glm::translate(glm::identity<glm::mat4>(), -position);
}

glm::mat4 Camera::getRotationMatrix() const {
    glm::mat4 viewMatrix = glm::identity<glm::mat4>();

    viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.z), { 0, 0, 1 });
    viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.y), { 0, 1, 0 });
    viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.x), { 1, 0, 0 });

    return viewMatrix;
}
