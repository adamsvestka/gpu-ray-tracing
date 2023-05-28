#include "primitives.hpp"

glm::mat3 rotationMatrix(const glm::vec3 &rotation) {
    glm::mat3 rx = glm::mat3(
        1, 0, 0,
        0, cos(rotation.x), -sin(rotation.x),
        0, sin(rotation.x), cos(rotation.x)
    );

    glm::mat3 ry = glm::mat3(
        cos(rotation.y), 0, sin(rotation.y),
        0, 1, 0,
        -sin(rotation.y), 0, cos(rotation.y)
    );

    glm::mat3 rz = glm::mat3(
        cos(rotation.z), -sin(rotation.z), 0,
        sin(rotation.z), cos(rotation.z), 0,
        0, 0, 1
    );

    return rx * ry * rz;
}
