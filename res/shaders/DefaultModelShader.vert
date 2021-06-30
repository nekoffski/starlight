#version 330 core

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iTexturePosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;

out vec2 texturePosition;
out vec3 fragmentPosition;
out vec3 normal;
out vec4 fragmentPositionLightSpace;

void main() {
    texturePosition = iTexturePosition;
    normal = vec3(transpose(inverse(modelMatrix)) * vec4(iNormal, 1.0f));
    fragmentPosition = vec3(modelMatrix * vec4(iPosition, 1.0f));
    fragmentPositionLightSpace = lightSpaceMatrix * vec4(fragmentPosition, 1.0);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(iPosition, 1.0);
}