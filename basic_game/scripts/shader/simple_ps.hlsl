cbuffer MaterialCBuffer : register(b0)
{
    float4 Diffuse;
    float4 Specular;
    float Shininess;
    float3 Padding;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Col : COLOR;
    float3 Normal : NORMAL;
    float3 WorldPos : TEXCOORD0;
    float2 Tex : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return Diffuse;
}