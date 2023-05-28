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
    friend class ItemRef<Light, PointLight>;

public:
    LightBuffer(GLuint bufferTextureID, GLuint bufferSizeID, GLuint textureID);

    PointLightRef addPointLight(const glm::vec3 &position, float intensity, const glm::vec3 &color);
    DirectionalLightRef addDirectionalLight(const glm::vec3 &direction, float intensity, const glm::vec3 &color);
    GlobalLightRef addGlobalLight(float intensity, const glm::vec3 &color);

    void print() const;
};

class PointLightRef : public ItemRef<Light, PointLight> {
public:
    PointLightRef(LightBuffer &buffer, size_t index) : ItemRef(buffer, index, &Light::pointLight) {}

    glm::vec3 getPosition() { return get().position; }
    float getIntensity() { return get().intensity; }
    glm::vec3 getColor() { return get().color; }

    void setPosition(const glm::vec3 &position) { get().position = position; update(); }
    void setIntensity(int intensity) { get().intensity = intensity; update(); }
    void setColor(const glm::vec3 &color) { get().color = color; update(); }
};

class DirectionalLightRef : public ItemRef<Light, DirectionalLight> {
public:
    DirectionalLightRef(LightBuffer &buffer, size_t index) : ItemRef(buffer, index, &Light::directionalLight) {}

    glm::vec3 getDirection() { return get().direction; }
    float getIntensity() { return get().intensity; }
    glm::vec3 getColor() { return get().color; }

    void setDirection(const glm::vec3 &direction) { get().direction = direction; buffer.update(index); }
    void setIntensity(int intensity) { get().intensity = intensity; buffer.update(index); }
    void setColor(const glm::vec3 &color) { get().color = color; buffer.update(index); }
};

class GlobalLightRef : public ItemRef<Light, GlobalLight> {
public:
    GlobalLightRef(LightBuffer &buffer, size_t index) : ItemRef(buffer, index, &Light::globalLight) {}

    float getIntensity() { return get().intensity; }
    glm::vec3 getColor() { return get().color; }

    void setIntensity(float intensity) { get().intensity = intensity; buffer.update(index); }
    void setColor(const glm::vec3 &color) { get().color = color; buffer.update(index); }
};
