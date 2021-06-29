#version 440 core

layout(location = 0) in vec3 iPosition;

out vec3 textureCoordinates;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() {
    mat4 fixedViewMatrix = mat4(mat3(viewMatrix));

    textureCoordinates = iPosition;
    vec4 skyPosition = projectionMatrix * fixedViewMatrix * vec4(iPosition, 1.0f);

    gl_Position = skyPosition.xyww;
}