#include "camera.h"
#include "input_manager.h"
#include "euler_converter.h"

Camera::Camera()
    : m_position(0, 0.5, 0)
    , m_rotation(0, 0, 0, 0)
{
}

Camera::~Camera()
{

}

void Camera::Update()
{
	calcAxis();
}

/*
 *    View変換行列取得
 */
DirectX::XMMATRIX Camera::GetViewMatrix() const
{
    DirectX::XMVECTOR eye = DirectX::XMLoadFloat3(&m_position);
    DirectX::XMVECTOR forward = DirectX::XMLoadFloat3(&m_forward);
    DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_up);

    DirectX::XMVECTOR focus =
        DirectX::XMVectorAdd(eye, forward);

    return DirectX::XMMatrixLookAtLH(eye, focus, up);
}

void Camera::SetPosition(const DirectX::XMFLOAT3& pos)
{
    m_position.x = pos.x;
    m_position.y = pos.y;
    m_position.z = pos.z;
}

void Camera::SetRotation(const DirectX::XMFLOAT4& rot)
{
	m_rotation = rot;
}

void Camera::calcAxis()
{
    // 回転行列
    DirectX::XMMATRIX rotMat =
        DirectX::XMMatrixRotationQuaternion(
            DirectX::XMLoadFloat4(&m_rotation)
        );

    // 基準ベクトル
    DirectX::XMVECTOR forward = DirectX::XMVectorSet(0, 0, 1, 0);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);

    // 回転を適用
    forward = DirectX::XMVector3TransformNormal(forward, rotMat);
    up = DirectX::XMVector3TransformNormal(up, rotMat);

    DirectX::XMVECTOR right = DirectX::XMVector3Cross(up, forward);

    forward = DirectX::XMVector3Normalize(forward);
    up = DirectX::XMVector3Normalize(up);
    right = DirectX::XMVector3Normalize(right);

    DirectX::XMStoreFloat3(&m_forward, forward);
    DirectX::XMStoreFloat3(&m_up, up);
    DirectX::XMStoreFloat3(&m_right, right);
}

DirectX::XMFLOAT4 Camera::GetEyePos() const
{
    return DirectX::XMFLOAT4(m_position.x, m_position.y, m_position.z, 1.f);
}