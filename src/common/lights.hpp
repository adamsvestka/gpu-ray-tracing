#pragma once

#include <sstream>

#include <glm/glm.hpp>

const unsigned __lights_stride = 3;

#pragma pack(push, 1)

struct PointLight {
private:
    float type = 1;
    float stride = __lights_stride;

public:
    glm::vec3 position;
    float intensity;
    glm::vec3 color;

    PointLight(const glm::vec3 &position, float intensity, const glm::vec3 &color) : position(position), intensity(intensity), color(color) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "PointLight("
            << "position: vec3(" << position.x << ", " << position.y << ", " << position.z << "), "
            << "intensity: " << intensity << ", "
            << "color: vec3(" << color.x << ", " << color.y << ", " << color.z << ")"
            << ")";
        return ss.str();
    }

private:
    float __padding[3];
};

struct DirectionalLight {
private:
    float type = 2;
    float stride = __lights_stride;

public:
    glm::vec3 direction;
    float intensity;
    glm::vec3 color;

    DirectionalLight(const glm::vec3 &direction, float intensity, const glm::vec3 &color) : direction(direction), intensity(intensity), color(color) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "DirectionalLight("
            << "direction: vec3(" << direction.x << ", " << direction.y << ", " << direction.z << "), "
            << "intensity: " << intensity << ", "
            << "color: vec3(" << color.x << ", " << color.y << ", " << color.z << ")"
            << ")";
        return ss.str();
    }

private:
    float __padding[3];
};

struct GlobalLight {
private:
    float type = 3;
    float stride = __lights_stride;

public:
    float intensity;
    glm::vec3 color;

    GlobalLight(float intensity, const glm::vec3 &color) : intensity(intensity), color(color) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "GlobalLight("
            << "intensity: " << intensity << ", "
            << "color: vec3(" << color.x << ", " << color.y << ", " << color.z << ")"
            << ")";
        return ss.str();
    }

private:
    float __padding[6];
};

#pragma pack(pop)

union Light {
    glm::vec4 data[__lights_stride];

    PointLight pointLight;
    DirectionalLight directionalLight;
    GlobalLight globalLight;
};
