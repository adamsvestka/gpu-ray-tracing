#pragma once

#include "buffer_base.hpp"
#include "lights.hpp"

class PointLightRef;
class DirectionalLightRef;
class GlobalLightRef;

class LightBuffer : public BufferBase<Light> {
private:
    friend class PointLightRef;
    friend class DirectionalLightRef;
    friend class GlobalLightRef;

public:
    LightBuffer(GLuint bufferTextureID, GLuint bufferSizeID, GLuint textureID);

    PointLightRef addPointLight(const glm::vec3 &position, float intensity, const glm::vec3 &color);
    DirectionalLightRef addDirectionalLight(const glm::vec3 &direction, float intensity, const glm::vec3 &color);
    GlobalLightRef addGlobalLight(float intensity, const glm::vec3 &color);

    void print() const;
};

class PointLightRef {
private:
    LightBuffer &buffer;
    size_t index;

    inline PointLight &getPointLight() { return buffer[index].pointLight; }

public:
    PointLightRef(LightBuffer &buffer, size_t index) : buffer(buffer), index(index) {}

    glm::vec3 getPosition() { return getPointLight().position; }
    float getIntensity() { return getPointLight().intensity; }
    glm::vec3 getColor() { return getPointLight().color; }

    void setPosition(const glm::vec3 &position) { getPointLight().position = position; buffer.update(index); }
    void setIntensity(int intensity) { getPointLight().intensity = intensity; buffer.update(index); }
    void setColor(const glm::vec3 &color) { getPointLight().color = color; buffer.update(index); }

    void remove() { buffer.remove(index); }
};

class DirectionalLightRef {
private:
    LightBuffer &buffer;
    size_t index;

    inline DirectionalLight &getDirectionalLight() { return buffer[index].directionalLight; }

public:
    DirectionalLightRef(LightBuffer &buffer, size_t index) : buffer(buffer), index(index) {}

    glm::vec3 getDirection() { return getDirectionalLight().direction; }
    float getIntensity() { return getDirectionalLight().intensity; }
    glm::vec3 getColor() { return getDirectionalLight().color; }

    void setDirection(const glm::vec3 &direction) { getDirectionalLight().direction = direction; buffer.update(index); }
    void setIntensity(int intensity) { getDirectionalLight().intensity = intensity; buffer.update(index); }
    void setColor(const glm::vec3 &color) { getDirectionalLight().color = color; buffer.update(index); }

    void remove() { buffer.remove(index); }
};

class GlobalLightRef {
private:
    LightBuffer &buffer;
    size_t index;

    inline GlobalLight &getGlobalLight() { return buffer[index].globalLight; }

public:
    GlobalLightRef(LightBuffer &buffer, size_t index) : buffer(buffer), index(index) {}

    float getIntensity() { return getGlobalLight().intensity; }
    glm::vec3 getColor() { return getGlobalLight().color; }

    void setIntensity(float intensity) { getGlobalLight().intensity = intensity; buffer.update(index); }
    void setColor(const glm::vec3 &color) { getGlobalLight().color = color; buffer.update(index); }

    void remove() { buffer.remove(index); }
};
