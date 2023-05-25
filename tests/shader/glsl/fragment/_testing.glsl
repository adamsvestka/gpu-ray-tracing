#version 330 core

#define M_PI float(3.141592653589793238462643383279)

out vec4 color;

uniform vec2 resolution;
uniform vec3 light;
uniform int lightStrength;

uniform samplerBuffer dataTexture;

#include "functions.decl.glsl"
#include "sphere.decl.glsl"

#include "functions.glsl"
#include "sphere.glsl"

#inject