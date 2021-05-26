#version 440 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 aTexPos;

out vec2 texPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 localModel;
uniform float scale;

vec4 calculateParticlePosition() {
    mat4 modelView = view * model * localModel;
    return projection * (modelView * vec4(0.0, 0.0, 0.0, 1.0) + vec4(iPos.xy * scale, 0.0, 0.0));
}

void main() {
    texPos = aTexPos;
    gl_Position = calculateParticlePosition();
}
