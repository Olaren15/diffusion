static const float2 positions[3] = {
    { 0.0, -0.5 },
    { 0.5, 0.5 },
    { -0.5, 0.5 }
};

static const float3 colors[3] = {
    { 1.0, 0.0, 0.0 },
    { 0.0, 1.0, 0.0 },
    { 0.0, 0.0, 1.0 }
};

struct vs_output_t {
    float4 position: SV_Position;
    float3 color: COLOR0;
};

[shader("vertex")]
vs_output_t vertex_main(uint vertex_index: SV_VertexID) {
    vs_output_t output = {};
    output.position = float4(positions[vertex_index], 0.0, 1.0);
    output.color = colors[vertex_index];

    return output;
}

[shader("fragment")]
float4 fragment_main(float3 color: COLOR0): SV_Target {
    return float4(color, 1.0);
}
