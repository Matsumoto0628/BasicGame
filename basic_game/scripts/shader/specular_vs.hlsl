cbuffer cbTransform : register(b0)
{
    matrix Transform; // World
};

cbuffer cbView : register(b1)
{
    matrix View;
};

cbuffer cbProjection : register(b2)
{
    matrix Projection;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Col : COLOR;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Col : COLOR;
    float3 Normal : NORMAL;
    float3 WorldPos : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    float4 pos = float4(input.Pos, 1.0f);
    float4 worldPos = mul(pos, Transform);
    worldPos = mul(worldPos, View);
    worldPos = mul(worldPos, Projection);
    output.Pos = worldPos;
    output.Normal = mul(input.Normal, (float3x3) Transform);
    output.WorldPos = worldPos.xyz;
    output.Col = input.Col;

    return output;
}
