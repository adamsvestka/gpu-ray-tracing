#version 330 core

#define M_PI float(3.141592653589793238462643383279)

out vec3 color;

uniform vec2 resolution;
uniform vec3 center;
uniform vec3 center2;
uniform vec3 light;
uniform int lightStrength;

#include "functions.decl.glsl"
#include "sphere.decl.glsl"

#include "functions.glsl"
#include "sphere.glsl"


void main() {
    // vec2 pos = gl_FragCoord.xy;
    // vec2 viewportDimensions = vec2(800, 600);
    // vec2 pos = mix(vec2(0, 0), vec2(1, 1), gl_FragCoord.xy / resolution.xy);

    vec3 direction = getCameraRay(gl_FragCoord.xy);

    // if (direction.y > 0) color = vec3(1, 0, 0);
    // else color = vec3(0, 1, 0);
    // color = vec3(direction.x > 0, direction.y > 0, direction.z > 0);
    Sphere sphere = Sphere_new(center, 3.0);
    Sphere sphere2 = Sphere_new(center2, 1.0);

    vec3 intersection = Sphere_intersect(sphere, vec3(0), direction);
    if (intersection == vec3(0)) color = vec3(0);
    else if (true || Sphere_intersect(sphere2, intersection, light - intersection) != vec3(0)) {
        vec3 normal = normalize(intersection - center);
        color = calculateLighting(intersection, normal, direction, lightStrength, 100);
    } else color = vec3(0.1);
    // color = vec3(1) - intersectSphere(vec3(0), direction) / 10;

    // if (gl_FragCoord.x / resolution.x > 0.5) color = vec3(1, 0, 0);
    // else color = vec3(0, 1, 0);

    // color = vec3(int(gl_FragCoord.x / 8.0) % 2, int(gl_FragCoord.y / 8.0) % 2, 0.0);
    // color = vec3(pos.xy, 0);
}
