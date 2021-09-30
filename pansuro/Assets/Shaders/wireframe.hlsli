cbuffer cbWorld : register(b0)
{
    row_major matrix gWorld;
}

cbuffer cbViewProj : register(b1)
{
    row_major matrix gViewProj;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

PSInput VSMain(VSInput input)
{
    PSInput result;

    result.position = mul(float4(input.position, 1.0f), gWorld);
    result.position = mul(result.position, gViewProj);
    result.uv = input.uv;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float4 color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    return color;
}
