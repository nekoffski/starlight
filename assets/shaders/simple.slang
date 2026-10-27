

struct VertexOutput {
    float4 position : SV_Position;
    float3 color : COLOR0;
};

struct Uniforms {
    float4 color;
    float4 padding;
};

ParameterBlock<Uniforms> uniforms;

[shader("vertex")]
VertexOutput vertexMain(float3 position, float3 color) {
    VertexOutput output;
    output.position = float4(position, 1.0);
    output.color = color;
    return output;
}

[shader("fragment")]
float4 fragmentMain(VertexOutput input) : SV_Target {
    return float4(input.color, 1.0);
}
