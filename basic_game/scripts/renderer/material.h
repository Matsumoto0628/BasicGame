#pragma once

struct MaterialData
{
    DirectX::XMFLOAT4 Diffuse;
    DirectX::XMFLOAT4 Specular;
    float Shininess;
};

class Material
{
public:
    MaterialData Data;
    ID3D11ShaderResourceView* DiffuseTex = nullptr;
};