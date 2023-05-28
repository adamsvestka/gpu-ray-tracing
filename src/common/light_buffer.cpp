#include "light_buffer.hpp"

#include <iostream>

LightBuffer::LightBuffer(GLuint bufferTextureID, GLuint bufferSizeID, GLuint textureID) : BufferBase(bufferTextureID, bufferSizeID, textureID) {}

PointLightRef LightBuffer::addPointLight(const glm::vec3 &position, float intensity, const glm::vec3 &color) {
    buffer.push_back({ pointLight: PointLight(position, intensity, color) });
    update();
    return PointLightRef(*this, buffer.size() - 1);
}

DirectionalLightRef LightBuffer::addDirectionalLight(const glm::vec3 &direction, float intensity, const glm::vec3 &color) {
    buffer.push_back({ directionalLight: DirectionalLight(direction, intensity, color) });
    update();
    return DirectionalLightRef(*this, buffer.size() - 1);
}

GlobalLightRef LightBuffer::addGlobalLight(float intensity, const glm::vec3 &color) {
    buffer.push_back({ globalLight: GlobalLight(intensity, color) });
    update();
    return GlobalLightRef(*this, buffer.size() - 1);
}

void LightBuffer::print() const {
    std::cout << "LightBuffer: " << buffer.size() << " lights" << std::endl;
    for (size_t i = 0; i < buffer.size(); i++) {
        switch ((int)buffer[i].data->x) {
            case 1:
                std::cout << "    " << buffer[i].pointLight.toString() << std::endl;
                break;
            case 2:
                std::cout << "    " << buffer[i].directionalLight.toString() << std::endl;
                break;
            case 3:
                std::cout << "    " << buffer[i].globalLight.toString() << std::endl;
                break;
        }
    }
}
