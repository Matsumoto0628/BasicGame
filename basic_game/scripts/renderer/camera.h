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
    void SetRotation(const DirectX::XMFLOAT4& rot); // pitch, yaw, roll
	DirectX::XMFLOAT3 GetForward() const { return m_forward; }
	DirectX::XMFLOAT3 GetRight() const { return m_right; }
	DirectX::XMFLOAT3 GetUp() const { return m_up; }
	DirectX::XMFLOAT4 GetRotation() const { return m_rotation; }

private:
    void calcAxis();

    DirectX::XMFLOAT3 m_position = { 0, 0, -5.f };
    DirectX::XMFLOAT4 m_rotation = { 0, 0, 0, 0 };

    DirectX::XMFLOAT3 m_forward = { 0, 0, 1 };
    DirectX::XMFLOAT3 m_up = { 0, 1, 0 };
    DirectX::XMFLOAT3 m_right = { 1, 0, 0 };
};