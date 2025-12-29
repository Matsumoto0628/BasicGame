#pragma once

struct CbTransform
{
	DirectX::XMFLOAT4X4 Transform;
};

struct CbTransformSet 
{
	CbTransform Data;
	ID3D11Buffer* pBuffer = nullptr;
};