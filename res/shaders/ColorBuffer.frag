#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
// uniform bool hdr;
// uniform float exposure;

void main() {
    const float exposure = 2.0;

    const float gamma = 2.2;
    vec2 texCoords = vec2(TexCoords.x, 1.0 - TexCoords.y);
    vec3 hdrColor = texture(hdrBuffer, texCoords).rgb;
    // if (hdr) {
    // reinhard
    vec3 result = hdrColor / (hdrColor + vec3(1.0));
    // exposure
    // vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it
    // result = pow(result, vec3(1.0 / gamma));
    // FragColor = vec4(1.0f);
    // } else {
    // vec3 result = pow(hdrColor, vec3(1.0 / gamma));
    // FragColor = vec4(1.0);

    FragColor = vec4(hdrColor, 1.0f);

    // }
}