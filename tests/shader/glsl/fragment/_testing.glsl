#version 330 core

#define M_PI float(3.141592653589793238462643383279)

out vec4 color;

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

#inject