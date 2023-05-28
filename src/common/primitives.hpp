#pragma once

#include <sstream>

#include <glm/glm.hpp>

class ShapeBuffer;

#pragma pack(push, 1)

struct Sphere {
private:
    float type = 1;
    float stride = 2;

public:
    glm::vec3 center;
    float radius;
    float radius2;

    Sphere(glm::vec3 center, float radius) : center(center), radius(radius), radius2(radius *radius) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "Sphere("
            << "center: vec3(" << center.x << ", " << center.y << ", " << center.z << "), "
            << "radius: " << radius << ", "
            << "radius2: " << radius2
            << ")";
        return ss.str();
    }

private:
    float __padding[1];
};

struct Cuboid {
private:
    float type = 2;
    float stride = 2;

public:
    glm::vec3 vmin, vmax;

    Cuboid(glm::vec3 center, glm::vec3 size) : vmin(center - size / 2.0f), vmax(center + size / 2.0f) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "Cuboid("
            << "vmin: vec3(" << vmin.x << ", " << vmin.y << ", " << vmin.z << "), "
            << "vmax: vec3(" << vmax.x << ", " << vmax.y << ", " << vmax.z << ")"
            << ")";
        return ss.str();
    }

private:
    float __padding[0];
};

#pragma pack(pop)

union Shape {
    glm::vec4 data[2];

    Sphere sphere;
    Cuboid cuboid;
};
