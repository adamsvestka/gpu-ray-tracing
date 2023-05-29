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

    PointLightRef addPointLight(const glm::vec3 &position, const glm::vec3 &color, float intensity);
    DirectionalLightRef addDirectionalLight(const glm::vec3 &direction, const glm::vec3 &color, float intensity);
    GlobalLightRef addGlobalLight(const glm::vec3 &color, float intensity);

    void print() const;
};

class PointLightRef : public ItemRef<Light, PointLight> {
public:
    PointLightRef(LightBuffer &buffer, size_t index) : ItemRef(buffer, index, &Light::pointLight) {}

    glm::vec3 getPosition() { return get().position; }
    glm::vec3 getColor() { return get().color; }
    float getIntensity() { return get().intensity; }

    void setPosition(const glm::vec3 &position) { get().position = position; update(); }
    void setColor(const glm::vec3 &color) { get().color = color; update(); }
    void setIntensity(int intensity) { get().intensity = intensity; update(); }
};

class DirectionalLightRef : public ItemRef<Light, DirectionalLight> {
public:
    DirectionalLightRef(LightBuffer &buffer, size_t index) : ItemRef(buffer, index, &Light::directionalLight) {}

    glm::vec3 getDirection() { return get().direction; }
    glm::vec3 getColor() { return get().color; }
    float getIntensity() { return get().intensity; }

    void setDirection(const glm::vec3 &direction) { get().direction = direction; buffer.update(index); }
    void setColor(const glm::vec3 &color) { get().color = color; buffer.update(index); }
    void setIntensity(int intensity) { get().intensity = intensity; buffer.update(index); }
};

class GlobalLightRef : public ItemRef<Light, GlobalLight> {
public:
    GlobalLightRef(LightBuffer &buffer, size_t index) : ItemRef(buffer, index, &Light::globalLight) {}

    glm::vec3 getColor() { return get().color; }
    float getIntensity() { return get().intensity; }

    void setColor(const glm::vec3 &color) { get().color = color; buffer.update(index); }
    void setIntensity(float intensity) { get().intensity = intensity; buffer.update(index); }
};
