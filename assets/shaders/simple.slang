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
};

[shader("vertex")]
VertexOutput vertexMain(float3 position, float3 normal, float2 texCoord) {
    VertexOutput output;
    output.position = mul(view.viewToClip,
        mul(view.worldToView, mul(transform.modelToWorld, float4(position, 1.0))));
    output.uv = texCoord;
    return output;
}

[shader("fragment")]
float4 fragmentMain(VertexOutput input) : SV_Target {
    return resources.textures[draw.textureIndex].Sample(
        resources.samplers[draw.samplerIndex], input.uv);
}
