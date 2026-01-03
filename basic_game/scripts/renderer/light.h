#pragma once

struct Light {
	DirectX::XMFLOAT4 LightDir;
	DirectX::XMFLOAT4 LightColor;
	DirectX::XMFLOAT4 EyePos;
};

struct LightSet {
	Light Data;
	ID3D11Buffer* pBuffer = nullptr;
};