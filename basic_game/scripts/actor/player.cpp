#include "player.h"
#include "input_manager.h"

Player::Player()
    : m_position(0, 0.5, 0)
    , m_rotation(0, 0, 0)
{
}

Player::~Player()
{

}

void Player::Initialize(Camera* pCamera)
{
	m_pCamera = pCamera;
}

void Player::Update()
{
    move();
    look();
    calcMoveAxis();

	m_pCamera->SetPosition(m_position);
	m_pCamera->SetRotation(m_rotation);
}

void Player::SetPosition(const DirectX::XMFLOAT3& pos)
{
    m_position.x = pos.x;
    m_position.y = pos.y;
    m_position.z = pos.z;
}

void Player::SetRotation(const DirectX::XMFLOAT3& rot)
{
    m_rotation.x = rot.x;
    m_rotation.y = rot.y;
    m_rotation.z = rot.z;
}

void Player::AddPosition(const DirectX::XMFLOAT3& dir, float amount)
{
    m_position.x += dir.x * amount;
    m_position.y += dir.y * amount;
    m_position.z += dir.z * amount;
}

void Player::AddRotation(const DirectX::XMFLOAT3& dir, float amount)
{
    m_rotation.x += dir.x * amount;
    m_rotation.y += dir.y * amount;
    m_rotation.z += dir.z * amount;
}

void Player::move()
{
    if (InputManager::Instance().GetKey('W'))
    {
        AddPosition(m_forward, 0.025f);
    }
    if (InputManager::Instance().GetKey('S'))
    {
        AddPosition(m_forward, -0.025f);
    }
    if (InputManager::Instance().GetKey('A'))
    {
        AddPosition(m_right, -0.025f);
    }
    if (InputManager::Instance().GetKey('D'))
    {
        AddPosition(m_right, 0.025f);
    }
}

void Player::look()
{
    POINT delta = InputManager::Instance().GetMouseDelta();
    float sensitivity = 0.0002f;

    m_rotation.y += delta.x * sensitivity; // Yaw
    m_rotation.x += delta.y * sensitivity; // Pitch

    // Pitch制限
    const float kMaxPitch = DirectX::XM_PIDIV2 - 0.01f;
    const float kMinPitch = -DirectX::XM_PIDIV2 + 0.01f;

    if (m_rotation.x > kMaxPitch) m_rotation.x = kMaxPitch;
    if (m_rotation.x < kMinPitch) m_rotation.x = kMinPitch;
}

void Player::calcMoveAxis()
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

    DirectX::XMStoreFloat3(&m_forward, moveForward);
    DirectX::XMStoreFloat3(&m_right, moveRight);
}