#pragma once

struct CbProjection
{
    DirectX::XMFLOAT4X4  Projection;
};

struct CbProjectionSet
{
    CbProjection    Data;
    ID3D11Buffer* pBuffer = nullptr;
};