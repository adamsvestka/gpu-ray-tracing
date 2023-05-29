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
    glm::vec3 color;
    float intensity;

    PointLight(const glm::vec3 &position, const glm::vec3 &color, float intensity) : position(position), color(color), intensity(intensity) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "PointLight("
            << "position: vec3(" << position.x << ", " << position.y << ", " << position.z << "), "
            << "color: vec3(" << color.x << ", " << color.y << ", " << color.z << "), "
            << "intensity: " << intensity
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
    glm::vec3 color;
    float intensity;

    DirectionalLight(const glm::vec3 &direction, const glm::vec3 &color, float intensity) : direction(direction), color(color), intensity(intensity) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "DirectionalLight("
            << "direction: vec3(" << direction.x << ", " << direction.y << ", " << direction.z << "), "
            << "color: vec3(" << color.x << ", " << color.y << ", " << color.z << "), "
            << "intensity: " << intensity
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
    glm::vec3 color;
    float intensity;

    GlobalLight(const glm::vec3 &color, float intensity) : color(color), intensity(intensity) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "GlobalLight("
            << "color: vec3(" << color.x << ", " << color.y << ", " << color.z << "), "
            << "intensity: " << intensity
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
