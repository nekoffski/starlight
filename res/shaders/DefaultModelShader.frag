#version 420 core

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 bloomColor;

in vec2 texturePosition;
in vec3 normal;
in vec3 fragmentPosition;
in vec4 fragmentPositionLightSpace;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
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

uniform float farPlane;

layout(binding = 0) uniform sampler2D textureSampler;
layout(binding = 1) uniform sampler2D shadowMap;

layout(binding = 3) uniform samplerCube omnidirectionalShadowMap;

uniform uint directionalLightsCount;
uniform uint pointLightsCount;
uniform uint textures;

uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform Material material;
uniform vec3 viewPos;

float calculateDirectionalLightShadow() {
    vec3 projectedCoordinates = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;
    projectedCoordinates = projectedCoordinates * 0.5 + 0.5;

    if (projectedCoordinates.z > 1.0)
        return 0.0f;

    float currentDepth = projectedCoordinates.z;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    const int kernelSize = 3;
    const int offset = (kernelSize - 1) / 2;
    const float bias = 0.005;

    float shadow = 0.0;
    for (int x = -offset; x <= offset; ++x) {
        for (int y = -offset; y <= offset; ++y) {
            float neighbourDepth = texture(shadowMap, projectedCoordinates.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias > neighbourDepth) ? 1.0 : 0.0;
        }
    }

    return shadow / (kernelSize * kernelSize);
}

float calculatePointLightShadow(PointLight light) {
    vec3 lightPosition = vec3(light.modelMatrix * vec4(light.position, 1.0f));
    vec3 fragmentToLight = fragmentPosition - lightPosition;

    float closestDepth = texture(omnidirectionalShadowMap, fragmentToLight).r * farPlane;
    float currentDepth = length(fragmentToLight);

    const float bias = 0.1;
    return currentDepth - bias > closestDepth ? 1.0 : 0.5;
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

vec4 calculateDirectionalLights(vec3 viewDirection) {
    vec3 totalLight = vec3(0.0f);

    for (uint i = 0u; i < directionalLightsCount; ++i) {
        DirectionalLight light = directionalLights[i];
        vec3 lightDirection = normalize(light.direction);

        vec3 ambient = calculateAmbient(light.color);
        vec3 diffuse = calculateDiffuse(lightDirection, light.color);
        vec3 specular = calculateSpecular(viewDirection, lightDirection, light.color);

        float shadow = calculateDirectionalLightShadow();

        totalLight += (ambient + (1.0f - shadow) * (diffuse + specular));
    }

    return vec4(totalLight, 1.0f);
}

vec4 calculatePointLights(vec3 viewDirection) {
    vec3 totalLight = vec3(0.0f);

    for (uint i = 0u; i < pointLightsCount; ++i) {
        PointLight light = pointLights[i];

        vec3 lightPosition = vec3(light.modelMatrix * vec4(light.position, 1.0f));
        vec3 lightDirection = normalize(lightPosition - fragmentPosition);

        vec3 ambient = calculateAmbient(light.color);
        vec3 diffuse = calculateDiffuse(lightDirection, light.color);
        vec3 specular = calculateSpecular(viewDirection, lightDirection, light.color);

        float fragmentDistance = length(lightPosition - fragmentPosition);
        float attenuationFactor = calculateAttenuation(fragmentDistance, light.attenuationA, light.attenuationB, light.attenuationC);

        float shadow = calculatePointLightShadow(light);

        totalLight += (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuationFactor;
    }

    return vec4(totalLight, 1.0f);
}

vec4 calculateLight() {
    vec3 viewDirection = normalize(viewPos - fragmentPosition);
    return calculateDirectionalLights(viewDirection) + calculatePointLights(viewDirection);
}

void main() {
    vec4 defaultColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 fragmentTexture = (textures > 0 ? texture(textureSampler, texturePosition) : defaultColor);

    fragmentColor = vec4(0.1, 0.1, 0.1, 1.0) + fragmentTexture * calculateLight();
    fragmentColor = fragmentColor;

    float brightness = dot(fragmentColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    bloomColor = brightness > 1.0 ? fragmentColor : vec4(0.0, 0.0, 0.0, 1.0);
}
