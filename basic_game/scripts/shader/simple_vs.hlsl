cbuffer cbTransform : register(b0)
{
    matrix Transform;
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
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    float4 pos = float4(input.Pos, 1.0f);
    pos = mul(pos, Transform);
    pos = mul(pos, View);
    pos = mul(pos, Projection);

    output.Pos = pos;
    return output;
}