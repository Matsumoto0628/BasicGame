#pragma once

class Camera
{
public:
    Camera();
    ~Camera();

    void Update();

    DirectX::XMMATRIX GetViewMatrix() const;

private:
    DirectX::XMFLOAT3 m_eyePos = { 0, 0, 0 };
    DirectX::XMFLOAT3 m_focutPos = { 0, 0, 0 };
    DirectX::XMFLOAT3 m_up = { 0, 1.f, 0 };
};