#pragma once

struct LightData {
	DirectX::XMFLOAT4 LightDir;
	DirectX::XMFLOAT4 LightColor;
	DirectX::XMFLOAT4 EyePos;
};

class Light {
public:
	LightData Data;
	ID3D11Buffer* pBuffer = nullptr;
};