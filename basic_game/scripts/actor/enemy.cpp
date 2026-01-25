#include "enemy.h"
#include "player.h"
#include "euler_converter.h"
#include "renderer.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(Renderer& renderer, Player* pPlayer)
{
	m_pRenderer = &renderer;
    m_pPlayer = pPlayer;

    const char* BOX_PATH = "models/box.fbx";
    m_boxBody.Setup(renderer, BOX_PATH);
    m_boxRight.Setup(renderer, BOX_PATH);
    m_boxLeft.Setup(renderer, BOX_PATH);

    m_collider.Initialize(renderer);
}

void Enemy::Setup()
{
    m_position = { 0.f, 0.5f, 3.f };
    m_rotation = { 0.f, 0.f, 0.f, 0.f };

    m_boxBody.SetPosition(m_position);
    m_boxLeft.SetPosition(m_position);
    m_boxRight.SetPosition(m_position);

    m_boxBody.SetScale({ 0.001f, 0.0005f, 0.002f });
    m_boxRight.SetScale({ 0.002f, 0.0001f, 0.001f });
    m_boxLeft.SetScale({ 0.002f, 0.0001f, 0.001f });

    m_boxRight.SetPivot({ -0.2f, 0, 0 });
    m_boxLeft.SetPivot({ 0.2f, 0, 0 });

    m_boxBody.ChangeColor({ 1, 1, 0, 1 });
    m_boxRight.ChangeColor({ 1, 1, 0, 0.5f });
    m_boxLeft.ChangeColor({ 1, 1, 0, 0.5f });

	m_boxBody.ChangeMaterial();
	m_boxRight.ChangeMaterial();
	m_boxLeft.ChangeMaterial();

    m_collider.SetRadius(0.2f);
	m_collider.SetActive(true);
}

void Enemy::Update()
{
    if (!m_isKnockback) 
    {
        move();
        look();
    }
    
    calcMoveAxis();
	calcBoxPos();
    calcBoxRot();

    m_collider.Update();
	m_collider.SetPosition(m_position);

    if (m_isHit) 
    {
		m_hitTimer += 0.017f;
        if (m_hitTimer > HIT_DURATION) 
        {
            m_isHit = false;
			m_hitTimer = 0.f;
			
            m_boxBody.ChangeColor({ 1, 1, 0, 1 });
            m_boxRight.ChangeColor({ 1, 1, 0, 0.5f });
            m_boxLeft.ChangeColor({ 1, 1, 0, 0.5f });

            m_boxBody.ChangeShader(&m_pRenderer->SpecularShader);
            m_boxRight.ChangeShader(&m_pRenderer->SpecularShader);
            m_boxLeft.ChangeShader(&m_pRenderer->SpecularShader);

            m_boxBody.ChangeMaterial();
            m_boxRight.ChangeMaterial();
            m_boxLeft.ChangeMaterial();
        }
    }

    if (m_isKnockback)
    {
        m_knockbackTimer += 0.017f;
        if (m_knockbackTimer > KNOCKBACK_DURATION)
        {
			m_collider.SetActive(true);
            m_isKnockback = false;
            m_knockbackTimer = 0.f;
        }

        m_position = DirectX::XMFLOAT3(
            m_position.x + m_forward.x * -0.01f,
            m_position.y + m_forward.y * -0.01f,
            m_position.z + m_forward.z * -0.01f);
    }
}

void Enemy::Draw()
{
    m_boxBody.Draw();
    m_boxRight.Draw();
    m_boxLeft.Draw();
    m_collider.Draw();
}

void Enemy::Terminate()
{
	m_boxBody.Terminate();
	m_boxRight.Terminate();
	m_boxLeft.Terminate();
}

void Enemy::move()
{
    m_position = DirectX::XMFLOAT3(
        m_position.x + m_forward.x * 0.01f,
        m_position.y + m_forward.y * 0.01f,
        m_position.z + m_forward.z * 0.01f);
}

float Enemy::lerpAngle(float a, float b, float t)
{
    float diff = fmodf(b - a + 540.f, 360.f) - 180.f;
    return a + diff * t;
}

void Enemy::look()
{
#if 1
    static float time = 0.0f;
    time += 0.01f;

    DirectX::XMFLOAT3 enemyPos = m_position;
    DirectX::XMFLOAT3 playerPos = m_pPlayer->GetPosition();

    DirectX::XMFLOAT3 dir{
        playerPos.x - enemyPos.x,
        playerPos.y - enemyPos.y,
        playerPos.z - enemyPos.z
    };

    float lenXZ = sqrtf(dir.x * dir.x + dir.z * dir.z);
    if (lenXZ < 0.0001f) return;

    // í èÌÇÃå¸Ç´åvéZ
    float baseYaw = atan2f(dir.x, dir.z) * DirectX::XMConvertToDegrees(1.0f);
    float basePitch = atan2f(dir.y, lenXZ) * DirectX::XMConvertToDegrees(1.0f);
    
    float amplitude = 20.0f;   // è„â∫ÇÃç≈ëÂäpìx
    float speed = 2.0f;    // óhÇÍÇÈë¨Ç≥
    float wavePitch = sinf(time * speed) * amplitude;

    m_yaw = baseYaw;
    m_pitch = basePitch + wavePitch;

    // êßå¿
    const float maxPitch = 89.f;
    const float minPitch = -89.f;
    if (m_pitch > maxPitch) m_pitch = maxPitch;
    if (m_pitch < minPitch) m_pitch = minPitch;

    m_rotation = EulerToQuaternion(
        DirectX::XMFLOAT3(m_pitch, m_yaw, 0.f)
    );
#else
    DirectX::XMFLOAT3 enemyPos = m_position;
    DirectX::XMFLOAT3 playerPos = m_pPlayer->GetPosition();

    DirectX::XMFLOAT3 dir{
        playerPos.x - enemyPos.x,
        playerPos.y - enemyPos.y,
        playerPos.z - enemyPos.z
    };

    DirectX::XMVECTOR vDir = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&dir));

    float targetYaw = atan2f(dir.x, dir.z);
    float lenXZ = sqrtf(dir.x * dir.x + dir.z * dir.z);
    float targetPitch = atan2f(dir.y, lenXZ);
    
    float turnSpeed = 0.1f; // è¨Ç≥Ç¢ÇŸÇ«ëÂâÒÇË

    m_yaw = lerpAngle(m_yaw, targetYaw * DirectX::XMConvertToDegrees(1), turnSpeed);
    m_pitch = lerpAngle(m_pitch, targetPitch * DirectX::XMConvertToDegrees(1), turnSpeed);

    m_rotation = EulerToQuaternion({ m_pitch, m_yaw, 0.f });
#endif
}

void Enemy::calcMoveAxis()
{
    // à⁄ìÆópâÒì]çsóÒÅiYawÇÃÇ›Åj
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

void Enemy::calcBoxPos()
{
	const float offset = 0.3f;
    DirectX::XMFLOAT3 right{
        m_position.x + m_right.x * offset,
        m_position.y + m_right.y * offset,
        m_position.z + m_right.z * offset,
    };

    DirectX::XMFLOAT3 left{
        m_position.x + m_right.x * -offset,
        m_position.y + m_right.y * -offset,
        m_position.z + m_right.z * -offset,
    };

    m_boxBody.SetPosition(m_position);
    m_boxRight.SetPosition(right);
    m_boxLeft.SetPosition(left);
}

void Enemy::calcBoxRot()
{
    // ñ{ëÃÇÃâÒì]
    DirectX::XMVECTOR q = DirectX::XMLoadFloat4(&m_rotation);

    // ÉçÅ[ÉJÉãâÒì]
    DirectX::XMFLOAT4 right = EulerToQuaternion(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
    DirectX::XMFLOAT4 left = EulerToQuaternion(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
    DirectX::XMVECTOR qRight = DirectX::XMLoadFloat4(&right);
    DirectX::XMVECTOR qLeft = DirectX::XMLoadFloat4(&left);

    // çáê¨
    qRight = DirectX::XMQuaternionMultiply(qRight, q);
    qLeft = DirectX::XMQuaternionMultiply(qLeft, q);

    DirectX::XMFLOAT4 rotRight;
    DirectX::XMFLOAT4 rotLeft;
    DirectX::XMStoreFloat4(&rotRight, qRight);
    DirectX::XMStoreFloat4(&rotLeft, qLeft);

    m_boxBody.SetRotation(m_rotation);
    m_boxRight.SetRotation(rotRight);
    m_boxLeft.SetRotation(rotLeft);

    static float t = 0.f;
    t += 0.15f;
    DirectX::XMFLOAT4 pivotRight = EulerToQuaternion(DirectX::XMFLOAT3(0.f, 0.f, 15 * cos(t)));
    DirectX::XMFLOAT4 pivotLeft = EulerToQuaternion(DirectX::XMFLOAT3(0.f, 0.f, -15 * cos(t)));
    m_boxRight.SetPivotRotation(pivotRight);
    m_boxLeft.SetPivotRotation(pivotLeft);
}

void Enemy::TakeDamage(int amount)
{
	m_health -= amount;
	if (m_health < 0) m_health = 0;
	knockback();

	m_collider.SetActive(false);
	m_isHit = true;

    m_boxBody.ChangeColor({ 1.f, 0.f, 0.f, 1 });
    m_boxRight.ChangeColor({ 1.f, 0.f, 0.f, 1 });
    m_boxLeft.ChangeColor({ 1.f, 0.f, 0.f, 1 });

    m_boxBody.ChangeShader(&m_pRenderer->SimpleShader);
    m_boxRight.ChangeShader(&m_pRenderer->SimpleShader);
    m_boxLeft.ChangeShader(&m_pRenderer->SimpleShader);

    m_boxBody.ChangeMaterial();
    m_boxRight.ChangeMaterial();
    m_boxLeft.ChangeMaterial();
}

void Enemy::knockback() 
{
    m_isKnockback = true;
}