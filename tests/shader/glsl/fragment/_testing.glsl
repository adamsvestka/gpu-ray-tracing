#version 330 core

out vec4 color;

#include "sphere.decl.glsl"
#include "sphere.glsl"

#inject

vec4 encode(int a) {
    return vec4(a, 0, 0, 0);
}

void main() {
    color = encode(test(1, 2));
}
