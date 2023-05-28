#include "shape_buffer.hpp"

#include <iostream>
#include <iomanip>

ShapeBuffer::ShapeBuffer(GLuint bufferTextureID, GLuint bufferSizeID) : bufferTextureID(bufferTextureID), bufferSizeID(bufferSizeID) {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, getBufferSize(), getBuffer().data(), GL_STATIC_DRAW);

    glGenTextures(1, &TBO);
    glBindTexture(GL_TEXTURE_BUFFER, TBO);
    glBufferData(GL_TEXTURE_BUFFER, getBufferSize(), NULL, GL_STATIC_DRAW);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, VBO);

    glUniform1i(bufferTextureID, 0);
    glUniform1i(bufferSizeID, buffer.size() * sizeof(Shape) / sizeof(glm::vec4));
}

const std::vector<glm::vec4> &ShapeBuffer::getBuffer() const {
    return reinterpret_cast<const std::vector<glm::vec4> &>(buffer);
}

GLuint ShapeBuffer::getBufferSize() const {
    return getBuffer().size() * sizeof(glm::vec4);
}

void ShapeBuffer::update() const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, getBufferSize(), getBuffer().data(), GL_STATIC_DRAW);

    glBindTexture(GL_TEXTURE_BUFFER, TBO);
    glBufferData(GL_TEXTURE_BUFFER, getBufferSize(), NULL, GL_STATIC_DRAW);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, VBO);

    glUniform1i(bufferTextureID, 0);
    glUniform1i(bufferSizeID, buffer.size() * sizeof(Shape) / sizeof(glm::vec4));
}

void ShapeBuffer::update(size_t index) const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(Shape), sizeof(Shape), &buffer[index]);

    glBindTexture(GL_TEXTURE_BUFFER, TBO);
    glBufferSubData(GL_TEXTURE_BUFFER, index * sizeof(Shape), sizeof(Shape), &buffer[index]);
}

const SphereRef &ShapeBuffer::addSphere(const glm::vec3 &position, float radius) {
    buffer.push_back({ sphere: Sphere(position, radius) });
    update();
    return SphereRef(*this, buffer.size() - 1);
}

Cuboid *ShapeBuffer::addCuboid(const glm::vec3 &position, glm::vec3 size) {
    buffer.push_back({ cuboid: Cuboid(position, size) });
    update();
    return &buffer.back().cuboid;
}

void ShapeBuffer::print() const {
    std::cout << "Shape count: " << buffer.size() << std::endl;
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

void ShapeBuffer::printBuffer() const {
    std::cout << "Buffer size: " << getBufferSize() << "B" << std::endl;
    auto &buf = getBuffer();
    for (size_t i = 0; i < buf.size(); i += 2) {
        std::cout << std::fixed << std::setprecision(2)
            << std::setfill(' ') << std::setw(8) << buf[i].x
            << std::setfill(' ') << std::setw(8) << buf[i].y
            << std::setfill(' ') << std::setw(8) << buf[i].z
            << std::setfill(' ') << std::setw(8) << buf[i].w
            << "    "
            << std::setfill(' ') << std::setw(8) << buf[i + 1].x
            << std::setfill(' ') << std::setw(8) << buf[i + 1].y
            << std::setfill(' ') << std::setw(8) << buf[i + 1].z
            << std::setfill(' ') << std::setw(8) << buf[i + 1].w
            << std::endl;
    }
}

void ShapeBuffer::printGPUBuffer(GLuint programID) const {
    GLint dataTextureSize;
    glGetUniformiv(programID, bufferSizeID, &dataTextureSize);
    std::cout << "GPU Buffer size: " << dataTextureSize << " * sizeof(vec4)" << std::endl;

    GLint size;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    std::vector<GLfloat> data(size / sizeof(GLfloat));
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, &data[0]);
    for (int i = 0; i < data.size(); i++) {
        std::cout << std::fixed << std::setprecision(2)
            << std::setfill(' ') << std::setw(8) << data[i];
        if (i % 8 == 7) std::cout << std::endl;
        else if (i % 8 == 3) std::cout << "    ";
    }
}

void ShapeBuffer::clear() {
    buffer.clear();
    update();
}

size_t ShapeBuffer::size() const {
    return buffer.size();
}

const Shape &ShapeBuffer::operator[](size_t index) const {
    return buffer[index];
}

Shape &ShapeBuffer::operator[](size_t index) {
    return buffer[index];
}

void ShapeBuffer::remove(size_t index) {
    buffer.erase(buffer.begin() + index);
    update();
}

void ShapeBuffer::remove(const Shape &shape) {
    for (size_t i = 0; i < buffer.size(); i++) {
        if (&buffer[i] == &shape) {
            remove(i);
            return;
        }
    }
}
