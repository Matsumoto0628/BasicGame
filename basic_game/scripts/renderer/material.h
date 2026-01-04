#pragma once

struct Shader;

struct Material
{
    DirectX::XMFLOAT4 Diffuse;
    DirectX::XMFLOAT4 Specular;
    float Shininess;
    float padding[3];
};

struct MaterialSet
{
    Material Data;
    ID3D11ShaderResourceView* DiffuseTex = nullptr;
    Shader* pShader;
    ID3D11Buffer* pBuffer = nullptr;
};