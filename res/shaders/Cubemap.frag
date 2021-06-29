#version 440 core

in vec3 textureCoordinates;
out vec4 fragmentColor;

layout(binding = 0) uniform samplerCube skyboxSampler;

void main() {
    fragmentColor = texture(skyboxSampler, textureCoordinates);
}
