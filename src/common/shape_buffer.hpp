#pragma once

#include <vector>

#include <GL/glew.h>

#include "primitives.hpp"

class SphereRef;
class CuboidRef;

class ShapeBuffer {
private:
    friend class SphereRef;
    friend class CuboidRef;

    std::vector<Shape> buffer;
    GLuint bufferTextureID, bufferSizeID;
    GLuint VBO, TBO;

    const std::vector<glm::vec4> &getBuffer() const;
    GLuint getBufferSize() const;

    void update() const;
    void update(size_t index) const;

    const Shape &operator[](size_t index) const;
    Shape &operator[](size_t index);

public:
    ShapeBuffer(GLuint bufferTextureID, GLuint bufferSizeID);

    SphereRef addSphere(const glm::vec3 &center, float radius, const Material &material);
    CuboidRef addCuboid(const glm::vec3 &center, glm::vec3 size, const Material &material);

    const glm::vec4 *data() const;
    size_t size() const;

    void clear();

    void remove(size_t index);
    void remove(const Shape &shape);

    void print() const;
    void printBuffer() const;
    void printGPUBuffer(GLuint programID) const;
};

class SphereRef {
private:
    ShapeBuffer &buffer;
    size_t index;

    inline Sphere &getSphere() { return buffer[index].sphere; }

public:
    SphereRef(ShapeBuffer &buffer, size_t index) : buffer(buffer), index(index) {}

    glm::vec3 getPosition() { return getSphere().getPosition(); }
    float getRadius() { return getSphere().radius; }
    Material &getMaterial() { return getSphere().material; }

    void setPosition(const glm::vec3 &position) { getSphere().setPosition(position); buffer.update(index); }
    void setRadius(float radius) { getSphere().radius = radius; buffer.update(index); }

    void move(const glm::vec3 &position) { getSphere().setPosition(getSphere().getPosition() + position); buffer.update(index); }
    void scale(float radius) { getSphere().radius *= radius; buffer.update(index); }

    void remove() { buffer.remove(index); }
};

class CuboidRef {
private:
    ShapeBuffer &buffer;
    size_t index;

    inline Cuboid &getCuboid() { return buffer[index].cuboid; }

public:
    CuboidRef(ShapeBuffer &buffer, size_t index) : buffer(buffer), index(index) {}

    glm::vec3 getPosition() { return getCuboid().getPosition(); }
    glm::vec3 getSize() { return getCuboid().getSize(); }
    Material &getMaterial() { return getCuboid().material; }

    void setPosition(const glm::vec3 &position) { getCuboid().setPosition(position); buffer.update(index); }
    void setSize(const glm::vec3 &size) { getCuboid().setSize(size); buffer.update(index); }
    void setRotation(const glm::vec3 &rotation) { getCuboid().setRotation(rotation); buffer.update(index); }

    void move(const glm::vec3 &position) { getCuboid().move(position); buffer.update(index); }
    void scale(const glm::vec3 &size) { getCuboid().scale(size); buffer.update(index); }
    void rotate(const glm::vec3 &rotation) { getCuboid().rotate(rotation); buffer.update(index); }

    void remove() { buffer.remove(index); }
};
