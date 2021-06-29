#version 440 core

in vec4 fragmentPosition;

uniform vec3 lightPosition;
uniform float farPlane;

void main() {
    float lightDistance = length(fragmentPosition.xyz - lightPosition);
    lightDistance = lightDistance / farPlane;

    gl_FragDepth = lightDistance;
}