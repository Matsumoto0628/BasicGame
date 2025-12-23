cbuffer cbTransform : register(b0)
{
    matrix Transform;
};

float4 main(float4 pos : POSITION) : SV_POSITION
{
    return mul(pos, Transform);
}