#version 330 core

#define M_PI float(3.141592653589793238462643383279)

out vec3 color;

uniform vec2 resolution;
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


const float scale = 1;

vec3 getCameraRay(vec2 pos) {
    float u = (2 * (pos.x + 0.5) / resolution.x - 1) * resolution.x / resolution.y * scale;
    float v = (1 - 2 * (pos.y + 0.5) / resolution.y) * scale;
    
    return normalize(vec3(1, u, v));
}

void main() {
    vec3 origin = (cameraTranslationMatrix * vec4(0, 0, 0, 1)).xyz;
    vec3 direction = (cameraRotationMatrix * vec4(getCameraRay(gl_FragCoord.xy), 0)).xyz;

    int i = 0;
    float dist = -1;
    vec3 normal = vec3(0);
    Material material;
    while (i < shapesBufferSize) {
        vec4 data = texelFetch(shapesBuffer, i);
        switch (int(data.x)) {
            case 1:
                Sphere sphere = Sphere_load(shapesBuffer, i);
                float newDist = Sphere_intersect(sphere, origin, direction);
                if (newDist != -1) {
                    if (newDist < dist || dist == -1) {
                        dist = newDist;
                        vec3 point = origin + direction * newDist;
                        normal = Sphere_normal(sphere, point);
                        material = sphere.material;
                    }
                }
                break;
            case 2:
                Cuboid cuboid = Cuboid_load(shapesBuffer, i);
                newDist = Cuboid_intersect(cuboid, origin, direction);
                if (newDist != -1) {
                    if (newDist < dist || dist == -1) {
                        dist = newDist;
                        vec3 point = origin + direction * newDist;
                        normal = Cuboid_normal(cuboid, point);
                        material = cuboid.material;
                    }
                }
                break;
        }
        i += int(data.y);
    }

    vec3 intersection = origin + direction * dist;
    if (dist != -1) {
        i = 0;
        color = vec3(0);
        while (i < lightsBufferSize) {
            vec4 data = texelFetch(lightsBuffer, i);
            switch (int(data.x)) {
                case 1:
                    PointLight pointLight = PointLight_load(lightsBuffer, i);
                    vec3 diffuse = PointLight_diffuse(pointLight, intersection, normal);
                    color += material.color * diffuse;
                    break;
                case 2:
                    DirectionalLight directionalLight = DirectionalLight_load(lightsBuffer, i);
                    diffuse = DirectionalLight_diffuse(directionalLight, intersection, normal);
                    color += material.color * diffuse;
                    break;
                case 3:
                    GlobalLight globalLight = GlobalLight_load(lightsBuffer, i);
                    diffuse = GlobalLight_diffuse(globalLight, intersection, normal);
                    color += material.color * diffuse;
                    break;
            }
            i += int(data.y);
        }
    } else {
        i = 0;
        color = vec3(0);
        while (i < lightsBufferSize) {
            vec4 data = texelFetch(lightsBuffer, i);
            switch (int(data.x)) {
                case 3:
                    GlobalLight globalLight = GlobalLight_load(lightsBuffer, i);
                    vec3 diffuse = GlobalLight_diffuse(globalLight, intersection, normal);
                    color += diffuse;
                    break;
            }
            i += int(data.y);
        }
    }

    // color = (20 - vec3(dist)) / 20;
}
