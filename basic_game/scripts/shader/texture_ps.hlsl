Texture2D DiffuseTex : register(t0);
SamplerState Samp : register(s0);

cbuffer MaterialCBuffer : register(b0)
{
    float4 Diffuse;
    float4 Specular;
    float Shininess;
};

cbuffer LightCBuffer : register(b1)
{
    float4 LightDir;
    float4 LightColor;
    float4 EyePos;
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
    float4 texColor = DiffuseTex.Sample(Samp, input.Tex);
    return float4(Diffuse.rgb * texColor.rgb, Diffuse.a);
}