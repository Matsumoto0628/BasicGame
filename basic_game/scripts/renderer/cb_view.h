#pragma once

/*
 *    View変換用コンスタントバッファ
 */
struct CbView
{
    DirectX::XMFLOAT4X4  View;
};

struct CbViewSet
{
    CbView  Data;
    ID3D11Buffer* pBuffer = nullptr;
};