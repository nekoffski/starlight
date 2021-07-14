#version 440 core

layout(location = 0) in vec3 iPosition;
layout(location = 2) in vec2 iTextureCoordinates;

out vec2 textureCoordinates;

void main() {
    textureCoordinates = iTextureCoordinates;
    gl_Position = vec4(iPosition, 1.0);
}