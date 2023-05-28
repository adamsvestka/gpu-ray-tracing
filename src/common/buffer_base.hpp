#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

template <typename T>
class BufferBase {
protected:
    std::vector<T> buffer;
    GLuint bufferTextureID, bufferSizeID, textureID;
    GLuint VBO, TBO;

    const std::vector<glm::vec4> &getBuffer() const;
    GLuint getBufferSize() const;

    void update() const;
    void update(size_t index) const;

    const T &operator[](size_t index) const;
    T &operator[](size_t index);

public:
    BufferBase(GLuint bufferTextureID, GLuint bufferSizeID, GLuint textureID);

    const glm::vec4 *data() const;
    size_t size() const;

    void clear();

    void remove(size_t index);
    void remove(const T &shape);

    void printBuffer() const;
    void printGPUBuffer(GLuint programID) const;
};


#include <iostream>
#include <iomanip>

template <typename T>
BufferBase<T>::BufferBase(GLuint bufferTextureID, GLuint bufferSizeID, GLuint textureID) : bufferTextureID(bufferTextureID), bufferSizeID(bufferSizeID), textureID(textureID) {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, getBufferSize(), getBuffer().data(), GL_STATIC_DRAW);

    glGenTextures(1, &TBO);
    glActiveTexture(GL_TEXTURE0 + textureID);
    glBindTexture(GL_TEXTURE_BUFFER, TBO);
    glBufferData(GL_TEXTURE_BUFFER, getBufferSize(), NULL, GL_STATIC_DRAW);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, VBO);

    glUniform1i(bufferTextureID, textureID);
    glUniform1i(bufferSizeID, buffer.size() * sizeof(T) / sizeof(glm::vec4));
}

template <typename T>
const std::vector<glm::vec4> &BufferBase<T>::getBuffer() const {
    return reinterpret_cast<const std::vector<glm::vec4> &>(buffer);
}

template <typename T>
GLuint BufferBase<T>::getBufferSize() const {
    return getBuffer().size() * sizeof(glm::vec4);
}

template <typename T>
void BufferBase<T>::update() const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, getBufferSize(), getBuffer().data(), GL_STATIC_DRAW);

    glUniform1i(bufferSizeID, buffer.size() * sizeof(T) / sizeof(glm::vec4));
}

template <typename T>
void BufferBase<T>::update(size_t index) const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(T), sizeof(T), &buffer[index]);
}

template <typename T>
const T &BufferBase<T>::operator[](size_t index) const {
    return buffer[index];
}

template <typename T>
T &BufferBase<T>::operator[](size_t index) {
    return buffer[index];
}

template <typename T>
const glm::vec4 *BufferBase<T>::data() const {
    return reinterpret_cast<const glm::vec4 *>(buffer.data());
}

template <typename T>
size_t BufferBase<T>::size() const {
    return buffer.size();
}

template <typename T>
void BufferBase<T>::clear() {
    buffer.clear();
    update();
}

template <typename T>
void BufferBase<T>::remove(size_t index) {
    buffer.erase(buffer.begin() + index);
    update();
}

template <typename T>
void BufferBase<T>::remove(const T &shape) {
    for (size_t i = 0; i < buffer.size(); i++) {
        if (&buffer[i] == &shape) {
            remove(i);
            return;
        }
    }
}

template <typename T>
void BufferBase<T>::printBuffer() const {
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

template <typename T>
void BufferBase<T>::printGPUBuffer(GLuint programID) const {
    GLint bufferSize;
    glGetUniformiv(programID, bufferSizeID, &bufferSize);
    std::cout << "GPU Buffer size: " << bufferSize << " * sizeof(vec4)" << std::endl;

    GLint size;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    std::vector<GLfloat> data(size / sizeof(GLfloat));
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, &data[0]);
    for (int i = 0; i < data.size(); i++) {
        std::cout << std::fixed << std::setprecision(2)
            << std::setfill(' ') << std::setw(8) << data[i];
        if (i % 8 == 7) std::cout << std::endl;
        else if (i % 8 == 3) std::cout << "    ";
    }
    std::cout << std::endl;
}
