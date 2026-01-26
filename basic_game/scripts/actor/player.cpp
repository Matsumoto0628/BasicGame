#include "player.h"
#include "input_manager.h"
#include "camera.h"
#include "weapon.h"
#include "euler_converter.h"
#include "renderer_2d.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(Renderer& renderer, Renderer2D& renderer2D, Camera* pCamera, Weapon* pWeapon)
{
	m_pRenderer2d = &renderer2D;
	m_pCamera = pCamera;
	m_pWeapon = pWeapon;
    m_collider.Initialize(renderer);
}

void Player::Setup() 
{
	m_position = { 0.f, 0.5f, 0.f };
	m_rotation = { 0.f, 0.f, 0.f, 0.f };
    m_collider.SetRadius(0.2f);
    m_collider.SetActive(true);
}

void Player::Update()
{
    if (!m_isKnockback)
    {
        move();
        look();
        if (InputManager::Instance().GetKeyDown(VK_LBUTTON))
        {
            m_pWeapon->SetAttackPos({ m_position.x + m_pCamera->GetForward().x * 0.5f,
                m_position.y + m_pCamera->GetForward().y * 0.5f,
                m_position.z + m_pCamera->GetForward().z * 0.5f });
            m_pWeapon->Slash();
        }
    }
    
    calcMoveAxis();

	m_pCamera->SetPosition(m_position);
	m_pCamera->SetRotation(m_rotation);

	calcWeaponPos();
    calcWeaponRot();

    m_collider.Update();
    m_collider.SetPosition(m_position);

    if (m_isKnockback)
    {
        m_knockbackTimer += 0.017f;
        if (m_knockbackTimer > KNOCKBACK_DURATION)
        {
            m_isKnockback = false;
            m_knockbackTimer = 0.f;
        }

        m_position = DirectX::XMFLOAT3(
            m_position.x + m_forward.x * -0.02f,
            m_position.y + m_forward.y * -0.02f,
            m_position.z + m_forward.z * -0.02f);
    }

    if (m_isHit) 
    {
		m_hitTimer += 0.017f;
		if (m_hitTimer > HIT_DURATION) {
			m_isHit = false;
			m_hitTimer = 0.f;
			m_collider.SetActive(true);
            m_pRenderer2d->SwitchImage(0, false);
		}
    }
}

void Player::Draw() 
{
    m_collider.Draw();
}

void Player::Terminate()
{
}

void Player::move()
{
    if (InputManager::Instance().GetKey('W'))
    {
        m_position = DirectX::XMFLOAT3(
            m_position.x + m_forward.x * 0.025f,
            m_position.y + m_forward.y * 0.025f,
            m_position.z + m_forward.z * 0.025f);
    }
    if (InputManager::Instance().GetKey('S'))
    {
        m_position = DirectX::XMFLOAT3(
            m_position.x + m_forward.x * -0.025f,
            m_position.y + m_forward.y * -0.025f,
            m_position.z + m_forward.z * -0.025f);
    }
    if (InputManager::Instance().GetKey('A'))
    {
        m_position = DirectX::XMFLOAT3(
            m_position.x + m_right.x * -0.025f,
            m_position.y + m_right.y * -0.025f,
            m_position.z + m_right.z * -0.025f);
    }
    if (InputManager::Instance().GetKey('D'))
    {
        m_position = DirectX::XMFLOAT3(
            m_position.x + m_right.x * 0.025f,
            m_position.y + m_right.y * 0.025f,
            m_position.z + m_right.z * 0.025f);
    }

    const float radius = 9.0f;

    float distSq = m_position.x * m_position.x +
        m_position.z * m_position.z;  // XZ平面だけ

    if (distSq > radius * radius)
    {
        float dist = sqrtf(distSq);
        float scale = radius / dist;

        m_position.x *= scale;
        m_position.z *= scale;
    }
}

void Player::look()
{
    POINT delta = InputManager::Instance().GetMouseDelta();
    const float sensitivity = 0.02f;

    m_yaw += delta.x * sensitivity;
    m_pitch += delta.y * sensitivity;

    // Pitch制限
    const float maxPitch = 89.f;
    const float minPitch = -89.f;

    if (m_pitch > maxPitch) m_pitch = maxPitch;
    if (m_pitch < minPitch) m_pitch = minPitch;

	m_rotation = EulerToQuaternion(DirectX::XMFLOAT3(m_pitch, m_yaw, 0.f));
}

void Player::calcMoveAxis()
{
    // 移動用回転行列（Yawのみ）
    DirectX::XMFLOAT4 rot = EulerToQuaternion(DirectX::XMFLOAT3(0.f, m_yaw, 0.f));
    DirectX::XMMATRIX yawMat =
        DirectX::XMMatrixRotationQuaternion(
            DirectX::XMLoadFloat4(&rot)
        );

    DirectX::XMVECTOR moveForward = DirectX::XMVectorSet(0, 0, 1, 0);
    DirectX::XMVECTOR moveRight = DirectX::XMVectorSet(1, 0, 0, 0);

    moveForward = DirectX::XMVector3TransformNormal(moveForward, yawMat);
    moveRight = DirectX::XMVector3TransformNormal(moveRight, yawMat);

    moveForward = DirectX::XMVector3Normalize(moveForward);
    moveRight = DirectX::XMVector3Normalize(moveRight);

    DirectX::XMStoreFloat3(&m_forward, moveForward);
    DirectX::XMStoreFloat3(&m_right, moveRight);
}

void Player::calcWeaponPos() 
{
    DirectX::XMFLOAT3 f = m_pCamera->GetForward();
    DirectX::XMFLOAT3 r = m_pCamera->GetRight();
    DirectX::XMFLOAT3 u = m_pCamera->GetUp();

    DirectX::XMFLOAT3 weaponPos{
		m_position.x + f.x * 0.5f + r.x * 0.15f + u.x * -0.1f,
        m_position.y + f.y * 0.5f + r.y * 0.15f + u.y * -0.1f,
		m_position.z + f.z * 0.5f + r.z * 0.15f + u.z * -0.1f,
    };

    m_pWeapon->SetPosition(weaponPos);
}

void Player::calcWeaponRot()
{
    // カメラ回転
    DirectX::XMFLOAT4 camRot = m_pCamera->GetRotation();
    DirectX::XMVECTOR qCam =
        DirectX::XMLoadFloat4(&camRot);

    // 武器ローカル回転（固定）
    DirectX::XMFLOAT4 weaponRot = EulerToQuaternion(DirectX::XMFLOAT3(45.f, -25.f, 0.f));
    DirectX::XMVECTOR qWeaponOffset = DirectX::XMLoadFloat4(&weaponRot);

    // 合成
    DirectX::XMVECTOR qFinal = DirectX::XMQuaternionMultiply(qWeaponOffset, qCam);

    DirectX::XMFLOAT4 rot;
    DirectX::XMStoreFloat4(&rot, qFinal);

    m_pWeapon->SetRotation(rot);
}

void Player::TakeDamage(int amount)
{
    m_pRenderer2d->SwitchImage(m_health / 10, false);
	m_health -= amount;
	if (m_health < 0)
	{
		m_health = 0;
	}
	knockback();
    m_collider.SetActive(false);

    m_isHit = true;
	m_pRenderer2d->SwitchImage(0, true);
}

void Player::knockback()
{
	m_isKnockback = true;
}