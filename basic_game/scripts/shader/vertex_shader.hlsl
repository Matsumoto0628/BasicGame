cbuffer cbTransform : register(b0)
{
    matrix Transform;
};

struct VS_INPUT
{
    float3 Pos : POSITION; // 頂点座標(モデル座標系)
    float4 Col : COLOR; // 頂点色
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Col : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    float4 pos = float4(input.Pos, 1.0);
    output.Pos = mul(pos, Transform);
    output.Col = input.Col;

    return output;
}