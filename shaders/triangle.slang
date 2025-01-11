struct scene_data_t {
    float4x4 view;
    float4x4 projection;
    float4x4 view_projection;
};

struct vs_intput_t {
    [[vk::location(0)]]
    float3 position : POSITION0;
    [[vk::location(1)]]
    float4 color : COLOR0;
};

struct vs_output_t {
    float4 position: SV_Position;
    [[vk::location(0)]]
     float4 color: COLOR0;
};

[[vk::binding(0, 0)]]
ConstantBuffer<scene_data_t> scene;

[shader("vertex")]
vs_output_t vertex_main(vs_intput_t input) {
    vs_output_t output = {};
    output.position = mul(scene.view_projection, float4(input.position, 1.0));
    output.color = input.color;

    return output;
}

[shader("fragment")]
float4 fragment_main([[vk::location(0)]] float4 color: COLOR0): SV_Target {
    return color;
}
