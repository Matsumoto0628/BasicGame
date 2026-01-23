cbuffer MaterialCBuffer : register(b0)
{
    float4 Diffuse;
    float4 Specular;
    float Shininess;
    float3 Padding;
};

float4 main() : SV_TARGET
{
    return Diffuse;
}