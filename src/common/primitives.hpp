#pragma once

#include <glm/glm.hpp>

#pragma pack(push, 1)

struct Sphere {
    const float type = 1;
    const float stride = 2;

    glm::vec3 center;
    float radius;
    float radius2;

    Sphere(glm::vec3 center, float radius) : center(center), radius(radius), radius2(radius * radius) {}

private:
    float padding[1];
};

struct Cuboid {
    const float type = 2;
    const float stride = 2;

    glm::vec3 vmin, vmax;

    Cuboid(glm::vec3 center, glm::vec3 size) : vmin(center - size / 2.0f), vmax(center + size / 2.0f) {}

private:
    float padding[0];
};

#pragma pack(pop)
