#version 420 core

out vec4 FragColor;

in vec2 texturePosition;
in vec3 normal;
in vec3 fragmentPosition;
in vec4 fragPosLightSpace;

struct DirectionalLights {
    vec3 direction;
    vec3 color;
};

struct PointLights {
    mat4 modelMatrix;
    vec3 position;
    vec3 color;
    float attenuationA;
    float attenuationB;
    float attenuationC;
};

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

#define MAX_DIRECTIONAL_LIGHTS 64
#define MAX_POINT_LIGHTS 64

uniform float far_plane;

layout(binding = 0) uniform sampler2D textureSampler;
layout(binding = 1) uniform sampler2D shadowMap;

layout(binding = 1) uniform samplerCube omnidirectionalShadowMap;

uniform uint directionalLightsNum;
uniform uint pointLightsNum;
uniform uint textures;

uniform DirectionalLights directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLights pointLights[MAX_POINT_LIGHTS];
uniform Material material;
uniform vec3 viewPos;

// float calculateShadows(vec4 fragPosLightSpace) {
//     vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//     projCoords = projCoords * 0.5 + 0.5;

//     if (projCoords.z > 0.45f)
//         return 0.0f;

//     const int kernelSize = 3;
//     const int offset = (kernelSize - 1) / 2;
//     const float bias = 0.005;

//     float currentDepth = projCoords.z;
//     vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

//     float shadow = 0.0;

//     for (int x = -offset; x <= offset; ++x) {
//         for (int y = -offset; y <= offset; ++y) {
//             float neighbourDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
//             shadow += (currentDepth - bias > neighbourDepth) ? 1.0 : 0.0;
//         }
//     }

//     return shadow / (kernelSize * kernelSize);
//     // return 0;
// }

float calculateShadows(vec4 fragPosLightSpace) {
    // get vector between fragment position and light position
    mat4 modelM = pointLights[0].modelMatrix;
    vec3 lightPosition = vec3(modelM * vec4(pointLights[0].position, 1.0f));

    vec3 fragToLight = lightPosition - fragmentPosition;
    fragToLight = -fragToLight;

    // use the light to fragment vector to sample from the depth map
    float closestDepth = texture(omnidirectionalShadowMap, fragToLight).r;

    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;

    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.01;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;

    // return 0.0;
}

float calculateAttenuation(float fragmentDistance, float a, float b, float c) {
    return 1.0f / (c + fragmentDistance * b + fragmentDistance * fragmentDistance * a);
}

vec3 calculateAmbient(vec3 lightColor) {
    const float ambientFactor = 0.2f;
    return lightColor * ambientFactor * material.ambientColor;
}

vec3 calculateDiffuse(vec3 lightDirection, vec3 lightColor) {
    float diff = max(dot(normal, normalize(lightDirection)), 0.0f);
    return lightColor * diff * material.diffuseColor;
}

vec3 calculateSpecular(vec3 viewDirection, vec3 lightDirection, vec3 lightColor) {
    const float specularFactor = 0.5f;

    vec3 reflected = normalize(reflect(lightDirection, normal));
    float spec = pow(max(dot(viewDirection, reflected), 0.0f), material.shininess);
    return specularFactor * spec * lightColor * material.specularColor;
}

vec4 calculateDirectionalLights(vec3 viewDirection, float shadow) {
    vec3 lightSum = vec3(0.0f);

    for (uint i = 0u; i < directionalLightsNum; ++i) {
        DirectionalLights light = directionalLights[i];

        vec3 lightDir = normalize(light.direction);

        vec3 ambient = calculateAmbient(light.color);
        vec3 diffuse = calculateDiffuse(lightDir, light.color);
        vec3 specular = calculateSpecular(viewDirection, lightDir, light.color);

        lightSum += (ambient + (1.0f - shadow) * (diffuse + specular));
    }

    return vec4(lightSum, 1.0f);
}

vec4 calculatePointLights(vec3 viewDirection, float shadow) {
    vec3 lightSum = vec3(0.0f);

    for (uint i = 0u; i < pointLightsNum; ++i) {
        PointLights light = pointLights[i];
        vec3 lightPosition = vec3(light.modelMatrix * vec4(light.position, 1.0f));
        vec3 lightDirection = normalize(lightPosition - fragmentPosition);

        vec3 ambient = calculateAmbient(light.color);
        vec3 diffuse = calculateDiffuse(lightDirection, light.color);
        vec3 specular = calculateSpecular(viewDirection, lightDirection, light.color);

        float fragmentDistance = length(lightPosition - fragmentPosition);
        float attenuationFactor = calculateAttenuation(fragmentDistance, light.attenuationA, light.attenuationB, light.attenuationC);

        lightSum += (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuationFactor;
    }

    return vec4(lightSum, 1.0f);
}

vec4 calculateLight(float shadow) {
    vec3 viewDirection = normalize(viewPos - fragmentPosition);
    return calculateDirectionalLights(viewDirection, shadow) + calculatePointLights(viewDirection, shadow);
}

void main() {
    vec4 defaultColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 color = (textures > 0 ? texture(textureSampler, texturePosition) : defaultColor);

    FragColor = color * calculateLight(calculateShadows(fragPosLightSpace));

    mat4 modelM = pointLights[0].modelMatrix;
    vec3 lightPosition = vec3(modelM * vec4(pointLights[0].position, 1.0f));

    vec3 fragToLight = fragmentPosition - lightPosition;

    // use the light to fragment vector to sample from the depth map
    float closestDepth = texture(omnidirectionalShadowMap, fragToLight).r;

    float currentDepth = length(fragToLight) / far_plane;
    // FragColor = vec4(vec3(closestDepth), 1.0);
    // FragColor = defaultColor;
}
