#version 450

layout (location = 0) in vec3 textureCoordinates;
layout (location = 0) out vec4 outColor;

layout (set = 1, binding = 0) uniform samplerCube samplers[1];

void main() {
    outColor = texture(samplers[0], textureCoordinates);
}
