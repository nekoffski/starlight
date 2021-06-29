#version 440

layout(location = 0) in vec3 iPosition;

uniform mat4 lightSpaceMatrix;
uniform mat4 modelMatrix;

void main() {
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(iPosition, 1.0);
}
