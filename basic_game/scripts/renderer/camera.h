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
    void AddPosition(const DirectX::XMFLOAT3& dir, float amount);
    void AddRotation(const DirectX::XMFLOAT3& dir, float amount); // pitch, yaw, roll

private:
    void move();
    void look();
    void calcAxis();
    void calcMoveAxis();

    DirectX::XMFLOAT3 m_position = { 0, 0, -5.f };
    DirectX::XMFLOAT3 m_rotation = { 0, 0, 0 }; // x=pitch y=yaw z=roll

    DirectX::XMFLOAT3 m_forward = { 0, 0, 1 };
    DirectX::XMFLOAT3 m_up = { 0, 1, 0 };
    DirectX::XMFLOAT3 m_right = { 1, 0, 0 };

	DirectX::XMFLOAT3 m_moveForward = { 0, 0, 1 };
	DirectX::XMFLOAT3 m_moveRight = { 1, 0, 0 };
};