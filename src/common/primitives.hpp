#pragma once

#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const unsigned __shapes_stride = 4;

glm::mat3 rotationMatrix(const glm::vec3 &rotation);

#pragma pack(push, 1)

struct Material {
    glm::vec3 color;
    float specular;
    float reflectivity;

    std::string toString() const {
        std::stringstream ss;
        ss << "Material("
            << "color: vec3(" << color.x << ", " << color.y << ", " << color.z << ")"
            << ")";
        return ss.str();
    }
};

struct Sphere {
private:
    float type = 1;
    float stride = __shapes_stride;

    glm::vec3 center;
public:
    float radius;
    float radius_squared;
    Material material;

    Sphere(const glm::vec3 &center, float radius, Material material) : center(center), radius(radius), radius_squared(radius *radius), material(material) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "Sphere("
            << "center: vec3(" << center.x << ", " << center.y << ", " << center.z << "), "
            << "radius: " << radius << ", "
            << "radius_squared: " << radius_squared << ", "
            << "material: " << material.toString()
            << ")";
        return ss.str();
    }

    const glm::vec3 &getPosition() const { return center; }
    void setPosition(const glm::vec3 &position) { center = position; }
    void move(const glm::vec3 &position) { center += position; }

private:
    float __padding[4];
};

struct Cuboid {
private:
    float type = 2;
    float stride = __shapes_stride;

    glm::vec3 vmin, vmax;
public:
    Material material;

    Cuboid(const glm::vec3 &center, const glm::vec3 &size, Material material) : vmin(center - size / 2.0f), vmax(center + size / 2.0f), material(material) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "Cuboid("
            << "vmin: vec3(" << vmin.x << ", " << vmin.y << ", " << vmin.z << "), "
            << "vmax: vec3(" << vmax.x << ", " << vmax.y << ", " << vmax.z << "), "
            << "material: " << material.toString()
            << ")";
        return ss.str();
    }

    glm::vec3 getPosition() const { return (vmin + vmax) / 2.0f; }
    void setPosition(const glm::vec3 &position) {
        glm::vec3 size = vmax - vmin;
        vmin = position - size / 2.0f;
        vmax = position + size / 2.0f;
    }
    void move(const glm::vec3 &position) {
        vmin += position;
        vmax += position;
    }

    glm::vec3 getSize() const { return vmax - vmin; }
    void setSize(const glm::vec3 &size) {
        glm::vec3 center = getPosition();
        vmin = center - size / 2.0f;
        vmax = center + size / 2.0f;
    }
    void scale(const glm::vec3 &size) {
        glm::vec3 center = getPosition();
        glm::vec3 half_size = getSize() / 2.0f;
        vmin = center - half_size * size;
        vmax = center + half_size * size;
    }

private:
    float __padding[1];
};

#pragma pack(pop)

union Shape {
    glm::vec4 data[__shapes_stride];

    Sphere sphere;
    Cuboid cuboid;
};
