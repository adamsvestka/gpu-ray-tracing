#pragma once

#include "buffer_base.hpp"
#include "primitives.hpp"

class SphereRef;
class CuboidRef;

class ShapeBuffer : public BufferBase<Shape> {
private:
    friend class SphereRef;
    friend class CuboidRef;

public:
    ShapeBuffer(GLuint bufferTextureID, GLuint bufferSizeID, GLuint textureID);

    SphereRef addSphere(const glm::vec3 &center, float radius, const Material &material);
    CuboidRef addCuboid(const glm::vec3 &center, glm::vec3 size, const Material &material);

    void print() const;
};

class SphereRef : public ItemRef<Shape, Sphere> {
public:
    SphereRef(ShapeBuffer &buffer, size_t index) : ItemRef(buffer, index, &Shape::sphere) {}

    glm::vec3 getPosition() { return get().getPosition(); }
    float getRadius() { return get().radius; }
    Material &getMaterial() { return get().material; }

    void setPosition(const glm::vec3 &position) { get().setPosition(position); buffer.update(index); }
    void setRadius(float radius) { get().radius = radius; buffer.update(index); }

    void move(const glm::vec3 &position) { get().setPosition(get().getPosition() + position); buffer.update(index); }
    void scale(float radius) { get().radius *= radius; buffer.update(index); }
};

class CuboidRef : public ItemRef<Shape, Cuboid> {
public:
    CuboidRef(ShapeBuffer &buffer, size_t index) : ItemRef(buffer, index, &Shape::cuboid) {}

    glm::vec3 getPosition() { return get().getPosition(); }
    glm::vec3 getSize() { return get().getSize(); }
    Material &getMaterial() { return get().material; }

    void setPosition(const glm::vec3 &position) { get().setPosition(position); buffer.update(index); }
    void setSize(const glm::vec3 &size) { get().setSize(size); buffer.update(index); }
    void setRotation(const glm::vec3 &rotation) { get().setRotation(rotation); buffer.update(index); }

    void move(const glm::vec3 &position) { get().move(position); buffer.update(index); }
    void scale(const glm::vec3 &size) { get().scale(size); buffer.update(index); }
    void rotate(const glm::vec3 &rotation) { get().rotate(rotation); buffer.update(index); }
};
