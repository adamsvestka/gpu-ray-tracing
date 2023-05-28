#pragma once

#include <vector>

#include <GL/glew.h>

#include "primitives.hpp"

class SphereRef;

class ShapeBuffer {
private:
    friend class SphereRef;

    std::vector<Shape> buffer;
    GLuint bufferTextureID, bufferSizeID;
    GLuint VBO, TBO;

    const std::vector<glm::vec4> &getBuffer() const;
    GLuint getBufferSize() const;

    void update() const;
    void update(size_t index) const;

public:
    ShapeBuffer(GLuint bufferTextureID, GLuint bufferSizeID);

    const SphereRef &addSphere(const glm::vec3 &center, float radius);
    Cuboid *addCuboid(const glm::vec3 &center, glm::vec3 size);

    void print() const;
    void printBuffer() const;
    void printGPUBuffer(GLuint programID) const;

    void clear();

    size_t size() const;

    const Shape &operator[](size_t index) const;
    Shape &operator[](size_t index);

    void remove(size_t index);
    void remove(const Shape &shape);
};

class SphereRef {
private:
    ShapeBuffer &buffer;
    size_t index;

    inline Sphere &getSphere() { return buffer[index].sphere; }

public:
    SphereRef(ShapeBuffer &buffer, size_t index) : buffer(buffer), index(index) {}

    const glm::vec3 &getCenter() { return getSphere().center; }
    float getRadius() { return getSphere().radius; }

    void setCenter(const glm::vec3 &center) { getSphere().center = center; buffer.update(index); }
    void setRadius(float radius) { getSphere().radius = radius; buffer.update(index); }

    void addCenter(const glm::vec3 &center) { getSphere().center += center; buffer.update(index); }
    void addRadius(float radius) { getSphere().radius += radius; buffer.update(index); }

    void multiplyCenter(const glm::vec3 &center) { getSphere().center *= center; buffer.update(index); }
    void multiplyRadius(float radius) { getSphere().radius *= radius; buffer.update(index); }

    void remove() { buffer.remove(index); }
};