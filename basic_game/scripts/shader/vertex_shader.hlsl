cbuffer cbTransform : register(b0)
{
    matrix Transform;
};

// Slot1 View変換
cbuffer cbView : register(b1)
{
    matrix View;
};

// Slot2 投影変換
cbuffer cbProjection : register(b2)
{
    matrix Projection;
};

struct VS_INPUT
{
    float3 Pos : POSITION; // 頂点座標(モデル座標系)
    float4 Col : COLOR; // 頂点色
    float3 Normal : NORMAL;
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

    float4 pos = float4(input.Pos, 1.0);
    float4 worldPos = mul(pos, Transform);
    worldPos = mul(worldPos, View);
    worldPos = mul(worldPos, Projection);
    output.Pos = worldPos;
    output.Normal = mul(input.Normal, (float3x3)Transform);
    output.WorldPos = worldPos.xyz;
    output.Col = input.Col;

    return output;
}