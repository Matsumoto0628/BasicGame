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
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 N = normalize(input.Normal);
    float3 L = normalize(-LightDir.xyz);
    float3 V = normalize(EyePos.xyz - input.WorldPos);

    // Lambert
    float diff = saturate(dot(N, L));
    diff = max(diff, 0.5f);

    // Phong Specular
    float3 R = reflect(-L, N);
    float spec = pow(saturate(dot(R, V)), Shininess);

    float3 color =
        Diffuse.rgb * diff +
        Specular.rgb * spec;
    
    return float4(color * LightColor.rgb, Diffuse.a);
}
