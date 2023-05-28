#include "shape_buffer.hpp"

#include <iostream>
#include <iomanip>

ShapeBuffer::ShapeBuffer(GLuint bufferTextureID, GLuint bufferSizeID, GLuint textureID) : BufferBase(bufferTextureID, bufferSizeID, textureID) {}

SphereRef ShapeBuffer::addSphere(const glm::vec3 &position, float radius, const Material &material) {
    buffer.push_back({ sphere: Sphere(position, radius, material) });
    update();
    return SphereRef(*this, buffer.size() - 1);
}

CuboidRef ShapeBuffer::addCuboid(const glm::vec3 &position, glm::vec3 size, const Material &material) {
    buffer.push_back({ cuboid: Cuboid(position, size, material) });
    update();
    return CuboidRef(*this, buffer.size() - 1);
}

void ShapeBuffer::print() const {
    std::cout << "ShapeBuffer: " << buffer.size() << " shapes" << std::endl;
    for (size_t i = 0; i < buffer.size(); i++) {
        switch ((int)buffer[i].data->x) {
            case 1:
                std::cout << "    " << buffer[i].sphere.toString() << std::endl;
                break;
            case 2:
                std::cout << "    " << buffer[i].cuboid.toString() << std::endl;
                break;
        }
    }
}
