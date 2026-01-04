#include "camera.h"
#include "input_manager.h"
#include "window.h"

Camera::Camera()
    : m_position(0, 0.5, 0)
    , m_rotation(0, 0, 0)
{
}

Camera::~Camera()
{

}

void Camera::Update()
{
    move();
    look();
	calcAxis();
    calcMoveAxis();
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

void Camera::SetRotation(const DirectX::XMFLOAT3& rot)
{
    m_rotation.x = rot.x;
    m_rotation.y = rot.y;
    m_rotation.z = rot.z;
}

void Camera::AddPosition(const DirectX::XMFLOAT3& dir, float amount) 
{
	m_position.x += dir.x * amount;
	m_position.y += dir.y * amount;
	m_position.z += dir.z * amount;
}

void Camera::AddRotation(const DirectX::XMFLOAT3& dir, float amount)
{
    m_rotation.x += dir.x * amount;
    m_rotation.y += dir.y * amount;
    m_rotation.z += dir.z * amount;
}

void Camera::move() 
{
    if (InputManager::Instance().GetKey('W'))
    {
        AddPosition(m_moveForward, 0.025f);
    }
    if (InputManager::Instance().GetKey('S'))
    {
        AddPosition(m_moveForward, -0.025f);
    }
    if (InputManager::Instance().GetKey('A'))
    {
        AddPosition(m_moveRight, -0.025f);
    }
    if (InputManager::Instance().GetKey('D'))
    {
        AddPosition(m_moveRight, 0.025f);
    }
}

void Camera::look()
{
    POINT delta = InputManager::Instance().GetMouseDelta();
    float sensitivity = 0.002f;

    m_rotation.y += delta.x * sensitivity; // Yaw
    m_rotation.x += delta.y * sensitivity; // Pitch
}

void Camera::calcAxis()
{
    // 回転行列（Yaw → Pitch → Roll）
    DirectX::XMMATRIX rotMat =
        DirectX::XMMatrixRotationRollPitchYaw(
            m_rotation.x,
            m_rotation.y,
            m_rotation.z
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

void Camera::calcMoveAxis() 
{
    // 移動用回転行列（Yawのみ）
    DirectX::XMMATRIX yawMat =
        DirectX::XMMatrixRotationY(m_rotation.y);

    DirectX::XMVECTOR moveForward = DirectX::XMVectorSet(0, 0, 1, 0);
    DirectX::XMVECTOR moveRight = DirectX::XMVectorSet(1, 0, 0, 0);

    moveForward = DirectX::XMVector3TransformNormal(moveForward, yawMat);
    moveRight = DirectX::XMVector3TransformNormal(moveRight, yawMat);

    moveForward = DirectX::XMVector3Normalize(moveForward);
    moveRight = DirectX::XMVector3Normalize(moveRight);

    DirectX::XMStoreFloat3(&m_moveForward, moveForward);
    DirectX::XMStoreFloat3(&m_moveRight, moveRight);
}