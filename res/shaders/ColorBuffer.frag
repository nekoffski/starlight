#version 440 core

out vec4 fragmentColor;

in vec2 textureCoordinates;

layout(binding = 1) uniform sampler2D hdrBuffer;
layout(binding = 2) uniform sampler2D bloomBuffer;

uniform float exposure;
uniform float gamma;

uniform int enableBloom = 0;

void main() {
    vec2 textureCoordinates = vec2(textureCoordinates.x, 1.0 - textureCoordinates.y);
    vec3 hdrColor = texture(hdrBuffer, textureCoordinates).rgb;

    if (enableBloom == 1)
        hdrColor += texture(bloomBuffer, textureCoordinates).rgb;

    // vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    // result = pow(result, vec3(1.0 / gamma));

    fragmentColor = vec4(hdrColor, 1.0f);
}