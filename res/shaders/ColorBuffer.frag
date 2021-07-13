#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 1) uniform sampler2D hdrBuffer;
layout(binding = 2) uniform sampler2D bloomBuffer;

uniform float exposure;
uniform float gamma;

void main() {
    vec2 texCoords = vec2(TexCoords.x, 1.0 - TexCoords.y);
    vec3 hdrColor = texture(hdrBuffer, texCoords).rgb;
    hdrColor += texture(bloomBuffer, texCoords).rgb;

    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    result = pow(result, vec3(1.0 / gamma));

    FragColor = vec4(result, 1.0f);
}