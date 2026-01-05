#pragma once

class Camera
{
public:
    Camera();
    ~Camera();

    void Update();
    DirectX::XMMATRIX GetViewMatrix() const;
    DirectX::XMFLOAT4 GetEyePos() const;
    void SetPosition(const DirectX::XMFLOAT3& pos);
    void SetRotation(const DirectX::XMFLOAT3& rot); // pitch, yaw, roll

private:
    void calcAxis();

    DirectX::XMFLOAT3 m_position = { 0, 0, -5.f };
    DirectX::XMFLOAT3 m_rotation = { 0, 0, 0 }; // x=pitch y=yaw z=roll

    DirectX::XMFLOAT3 m_forward = { 0, 0, 1 };
    DirectX::XMFLOAT3 m_up = { 0, 1, 0 };
    DirectX::XMFLOAT3 m_right = { 1, 0, 0 };
};