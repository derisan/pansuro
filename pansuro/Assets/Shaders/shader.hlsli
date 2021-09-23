cbuffer cbWorld : register(b0)
{
    float3 gOffset;
}

cbuffer cbViewProj : register(b1)
{
    row_major matrix gViewProj;
}

Texture2D tex0 : register(t0);
SamplerState sam0 : register(s0);

struct VSInput
{
    float3 position : POSITION;
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

    result.position = mul(float4(input.position + gOffset, 1.0f), gViewProj);
    result.uv = input.uv;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float4 color = tex0.Sample(sam0, input.uv);
    return color;
}
