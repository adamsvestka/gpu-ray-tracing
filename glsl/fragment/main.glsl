#version 330 core

#define M_PI float(3.141592653589793238462643383279)

out vec3 color;

uniform vec2 resolution;
uniform vec3 light;
uniform int lightStrength;

uniform samplerBuffer dataTexture;
uniform int dataTextureSize;

#include "functions.decl.glsl"
#include "sphere.decl.glsl"
#include "cuboid.decl.glsl"

#include "functions.glsl"
#include "sphere.glsl"
#include "cuboid.glsl"


void main() {
    vec3 direction = getCameraRay(gl_FragCoord.xy);

    int i = 0;
    vec3 intersection = vec3(-1);
    vec3 normal = vec3(0);
    while (i < dataTextureSize) {
        vec4 data = texelFetch(dataTexture, i);
        switch (int(data.x)) {
            case 1:
                Sphere sphere = Sphere_load(dataTexture, i);
                vec3 newIntersection = Sphere_intersect(sphere, vec3(0), direction);
                if (newIntersection != vec3(-1)) {
                    if (length(newIntersection) < length(intersection) || intersection == vec3(-1)) {
                        intersection = newIntersection;
                        normal = Sphere_normal(sphere, newIntersection);
                    }
                }
                break;
            case 2:
                Cuboid cuboid = Cuboid_load(dataTexture, i);
                newIntersection = Cuboid_intersect(cuboid, vec3(0), direction);
                if (newIntersection != vec3(-1)) {
                    if (length(newIntersection) < length(intersection) || intersection == vec3(-1)) {
                        intersection = newIntersection;
                        normal = Cuboid_normal(cuboid, newIntersection);
                    }
                }
                break;
        }
        i += int(data.y);
    }

    if (intersection != vec3(-1)) {
        color = calculateLighting(intersection, normal, direction, lightStrength, 100);
    } else color = vec3(0.02);
}
