// Resource table capacity matches renderer/core/Texture.hh.
struct Resources {
    Texture2D<float4> textures[64];
    SamplerState samplers[64];
};
ParameterBlock<Resources> resources;

struct DrawParameters {
    uint textureIndex;
    uint samplerIndex;
    uint2 padding;
};
ConstantBuffer<DrawParameters> draw;

struct TransformParameters {
    float4x4 modelToWorld;
};
ConstantBuffer<TransformParameters> transform;

struct ViewParameters {
    float4x4 worldToView;
    float4x4 viewToClip;
};
ConstantBuffer<ViewParameters> view;

struct VertexOutput {
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    float3 viewPosition : TEXCOORD1;
    float3 viewNormal : TEXCOORD2;
};

[shader("vertex")]
VertexOutput vertexMain(float3 position, float3 normal, float2 texCoord) {
    VertexOutput output;
    float4 viewPosition = mul(view.worldToView,
        mul(transform.modelToWorld, float4(position, 1.0)));
    output.position = mul(view.viewToClip, viewPosition);
    output.uv = texCoord;
    output.viewPosition = viewPosition.xyz;
    // The sandbox uses rotation and uniform scale.
    output.viewNormal = mul(view.worldToView,
        mul(transform.modelToWorld, float4(normal, 0.0))).xyz;
    return output;
}

[shader("fragment")]
float4 fragmentMain(VertexOutput input) : SV_Target {
    float3 normal = normalize(input.viewNormal);
    // shortcut: front-face rejection handles this convex cube; add depth testing for overlapping geometry.
    if (dot(normal, -input.viewPosition) <= 0.0)
        discard;
    float lighting = 0.25 + 0.75 *
        saturate(dot(normal, normalize(float3(0.4, 0.8, 1.0))));
    float4 color = resources.textures[draw.textureIndex].Sample(
        resources.samplers[draw.samplerIndex], input.uv);
    return float4(color.rgb * lighting, color.a);
}
