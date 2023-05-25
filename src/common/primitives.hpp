#pragma once

#include <glm/glm.hpp>

#pragma pack(push, 1)
struct Sphere {
    glm::vec3 center;
    float radius;
    float radius2;

    Sphere(glm::vec3 center, float radius) : center(center), radius(radius), radius2(radius *radius) {}

private:
    char padding[12];
};
#pragma pack(pop)
