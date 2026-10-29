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

struct VertexOutput {
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

[shader("vertex")]
VertexOutput vertexMain(float3 position, float3 color) {
    VertexOutput output;
    output.position = float4(position, 1.0);
    output.uv = position.xy + 0.5;
    return output;
}

[shader("fragment")]
float4 fragmentMain(VertexOutput input) : SV_Target {
    return resources.textures[draw.textureIndex].Sample(
        resources.samplers[draw.samplerIndex], input.uv);
}
