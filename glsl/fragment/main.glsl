#version 330 core

#define M_PI float(3.141592653589793238462643383279)

void swap(out float a, out float b) {
    float tmp = a;
    a = b;
    b = tmp;
}

out vec3 color;

uniform vec2 resolution;
uniform int renderMode;
uniform mat4 cameraTranslationMatrix;
uniform mat4 cameraRotationMatrix;

uniform samplerBuffer shapesBuffer;
uniform int shapesBufferSize;

uniform samplerBuffer lightsBuffer;
uniform int lightsBufferSize;

#include "material.decl.glsl"
#include "lights.decl.glsl"
#include "sphere.decl.glsl"
#include "cuboid.decl.glsl"

#include "sphere.glsl"
#include "cuboid.glsl"


const float scale = 0.5f;

vec3 getCameraRay(vec2 pos) {
    float u = (2 * (pos.x + 0.5) / resolution.x - 1) * resolution.x / resolution.y * scale;
    float v = (1 - 2 * (pos.y + 0.5) / resolution.y) * scale;
    
    return normalize(vec3(1, u, v));
}

struct Hit {
    float dist;
    vec3 normal;
    Material material;
    vec3 point;
};

Hit castRay(vec3 origin, vec3 direction) {
    int index = 0;
    Hit hit;
    hit.dist = -1;

    // ===== OBJECT INTERSECTION =====
    while (index < shapesBufferSize) {
        vec4 data = texelFetch(shapesBuffer, index);
        switch (int(data.x)) {
            case 1:
                Sphere sphere = Sphere_load(shapesBuffer, index);
                float newDist = Sphere_intersect(sphere, origin, direction);
                if (newDist != -1) {
                    if (newDist < hit.dist || hit.dist == -1) {
                        vec3 point = origin + direction * newDist;
                        vec3 normal = Sphere_normal(sphere, point);
                        hit = Hit(newDist, normal, sphere.material, point);
                    }
                }
                break;
            case 2:
                Cuboid cuboid = Cuboid_load(shapesBuffer, index);
                newDist = Cuboid_intersect(cuboid, origin, direction);
                if (newDist != -1) {
                    if (newDist < hit.dist || hit.dist == -1) {
                        vec3 point = origin + direction * newDist;
                        vec3 normal = Cuboid_normal(cuboid, point);
                        hit = Hit(newDist, normal, cuboid.material, point);
                    }
                }
                break;
        }
        index += int(data.y);
    }

    return hit;
}

vec3 calculateLighting(Hit hit, vec3 direction) {
    int index = 0;
    vec3 color = vec3(0);
    if (hit.dist != -1) {
        // ===== LIGHTING =====
        while (index < lightsBufferSize) {
            vec4 data = texelFetch(lightsBuffer, index);
            switch (int(data.x)) {
                case 1:
                    PointLight pointLight = PointLight_load(lightsBuffer, index);
                    vec3 diffuse = PointLight_diffuse(pointLight, hit.point, hit.normal);
                    if (length(diffuse) > 0) {
                        float shadowDist = castRay(hit.point + hit.normal * 0.001, pointLight.position - hit.point).dist;
                        if (shadowDist == -1 || shadowDist > length(pointLight.position - hit.point)) {
                            vec3 specular = PointLight_specular(pointLight, hit.point, hit.normal, direction, hit.material.shininess);
                            color += hit.material.color * diffuse * (1 - hit.material.reflectivity) + specular * hit.material.reflectivity;
                        }
                    }
                    break;
                case 2:
                    DirectionalLight directionalLight = DirectionalLight_load(lightsBuffer, index);
                    diffuse = DirectionalLight_diffuse(directionalLight, hit.point, hit.normal);
                    color += hit.material.color * diffuse;
                    break;
                case 3:
                    GlobalLight globalLight = GlobalLight_load(lightsBuffer, index);
                    diffuse = GlobalLight_diffuse(globalLight, hit.point, hit.normal);
                    color += hit.material.color * diffuse;
                    break;
            }
            index += int(data.y);
        }
    } else {
        // ===== BACKGROUND COLOR =====
        while (index < lightsBufferSize) {
            vec4 data = texelFetch(lightsBuffer, index);
            switch (int(data.x)) {
                case 3:
                    GlobalLight globalLight = GlobalLight_load(lightsBuffer, index);
                    vec3 diffuse = GlobalLight_diffuse(globalLight, hit.point, hit.normal);
                    color += diffuse;
                    break;
            }
            index += int(data.y);
        }
    }

    return color;
}

void main() {
    vec3 origin = (cameraTranslationMatrix * vec4(0, 0, 0, 1)).xyz;
    vec3 direction = (cameraRotationMatrix * vec4(getCameraRay(gl_FragCoord.xy), 0)).xyz;

    Hit hit = castRay(origin, direction);

    if (renderMode != 0 && hit.dist == -1) {
        color = vec3(0);
        return;
    }

    switch (renderMode) {
        case 1:
            color = abs(hit.normal);
            return;
        case 2:
            color = vec3(1.f - hit.dist / 100.f);
            return;
        case 3:
            color = vec3(hit.material.color);
            return;
    }

    color = calculateLighting(hit, direction);

    for (int i = 0; i < 3 && hit.dist != -1 && hit.material.reflectivity > 0; i++) {
        vec3 reflectDir = reflect(direction, hit.normal);
        Hit reflectHit = castRay(hit.point + hit.normal * 0.001, reflectDir);
        color = calculateLighting(reflectHit, reflectDir) * hit.material.reflectivity + color * (1 - hit.material.reflectivity);
        hit = reflectHit;
    }
}
