#include "enemy.h"
#include "player.h"
#include "euler_converter.h"
#include "renderer.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	Terminate();
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
    
	m_targetDuration = ((float)rand() / RAND_MAX) * 5.f;

	setTargetPosRandom();
}

void Enemy::Update()
{
    if (m_isDead) return;
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

    if (m_isAttack) 
    {
        m_targetPos = m_pPlayer->GetPosition();
        m_attackTimer += 0.017f;
        if (m_attackTimer > 10.f)
        {
			m_isAttack = false;
            m_attackTimer = 0.f;
            setTargetPosRandom();
        }
    }
    else 
    {
        m_targetTimer += 0.017f;
        if (m_targetTimer > m_targetDuration)
        {
            m_targetTimer = 0.f;
            m_targetDuration = ((float)rand() / RAND_MAX) * 5.f;
            setTargetPosRandom();
        }
    }

    if (m_health <= 0)
    {
		m_deadTimer += 0.017f;
        if (m_deadTimer > 0.5f) 
        {
			m_isDead = true;
        }
    }
}

void Enemy::Draw()
{
	if (m_isDead) return;
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
    float speed = 0.01f;
	if (m_moveType == Curve)
	{
		speed = 0.02f;
	}
	if (m_isAttack)
	{
		speed = 0.03f;
	}

    m_position = DirectX::XMFLOAT3(
        m_position.x + m_forward.x * speed,
        m_position.y + m_forward.y * speed,
        m_position.z + m_forward.z * speed);
}

float Enemy::lerpAngle(float a, float b, float t)
{
    float diff = fmodf(b - a + 540.f, 360.f) - 180.f;
    return a + diff * t;
}

void Enemy::look()
{
    switch (m_moveType)
    {
    case Curve:
		curveLook();
        break;
    case Basic:
		basicLook();
        break;
    default:
        break;
    }
}

void Enemy::basicLook() 
{
    m_waveTimer += 0.01f;

    DirectX::XMFLOAT3 enemyPos = m_position;

    DirectX::XMFLOAT3 dir{
        m_targetPos.x - enemyPos.x,
        m_targetPos.y - enemyPos.y,
        m_targetPos.z - enemyPos.z
    };

    float lenXZ = sqrtf(dir.x * dir.x + dir.z * dir.z);
    if (lenXZ < 0.0001f) return;

    // 通常の向き計算
    float baseYaw = atan2f(dir.x, dir.z) * DirectX::XMConvertToDegrees(1.0f);
    float basePitch = atan2f(dir.y, lenXZ) * DirectX::XMConvertToDegrees(1.0f);

    float amplitude = 20.0f;   // 上下の最大角度
    float speed = 2.0f;    // 揺れる速さ
    float wavePitch = sinf(m_waveTimer * speed) * amplitude;

    m_yaw = baseYaw;
    m_pitch = basePitch + wavePitch;

    // 制限
    const float maxPitch = 89.f;
    const float minPitch = -89.f;
    if (m_pitch > maxPitch) m_pitch = maxPitch;
    if (m_pitch < minPitch) m_pitch = minPitch;

    m_rotation = EulerToQuaternion(
        DirectX::XMFLOAT3(-m_pitch, m_yaw, 0.f)
    );
}

void Enemy::curveLook() 
{
    DirectX::XMFLOAT3 enemyPos = m_position;

    DirectX::XMFLOAT3 dir{
        m_targetPos.x - enemyPos.x,
        m_targetPos.y - enemyPos.y,
        m_targetPos.z - enemyPos.z
    };

    float lenXZ = sqrtf(dir.x * dir.x + dir.z * dir.z);
    if (lenXZ < 0.001f) return;

    // プレイヤー方向
    float baseYaw = atan2f(dir.x, dir.z) * DirectX::XMConvertToDegrees(1.0f);
    float basePitch = atan2f(dir.y, lenXZ) * DirectX::XMConvertToDegrees(1.0f);

    // 一定時間ごとに左右ランダム変更
    m_dirChangeTimer += 0.016f;
    if (m_dirChangeTimer > 1.5f) // 1.5秒ごとに進路変更
    {
        m_dirChangeTimer = 0.f;

        // -30°〜30° のランダムオフセット
        float r = (float)rand() / RAND_MAX;   // 0〜1
        m_sideYawOffset = (r * 60.f) - 60.f;  // -30〜+30
    }

    float targetYaw = baseYaw + m_sideYawOffset;

    float turnSpeed = 0.05f;
    m_yaw = lerpAngle(m_yaw, targetYaw, turnSpeed);
    m_pitch = lerpAngle(m_pitch, basePitch, turnSpeed);

    m_rotation = EulerToQuaternion({ -m_pitch, m_yaw, 0.f });
}

void Enemy::calcMoveAxis()
{
    DirectX::XMVECTOR q = DirectX::XMLoadFloat4(&m_rotation);
    DirectX::XMMATRIX mat = DirectX::XMMatrixRotationQuaternion(q);

    DirectX::XMVECTOR forward = DirectX::XMVectorSet(0, 0, 1, 0);
    forward = DirectX::XMVector3TransformNormal(forward, mat);
    forward = DirectX::XMVector3Normalize(forward);

    DirectX::XMVECTOR right = DirectX::XMVectorSet(1, 0, 0, 0);
    right = DirectX::XMVector3TransformNormal(right, mat);
    right = DirectX::XMVector3Normalize(right);

    DirectX::XMStoreFloat3(&m_forward, forward);
    DirectX::XMStoreFloat3(&m_right, right);
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
    // 本体の回転
    DirectX::XMVECTOR q = DirectX::XMLoadFloat4(&m_rotation);

    // ローカル回転
    DirectX::XMFLOAT4 right = EulerToQuaternion(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
    DirectX::XMFLOAT4 left = EulerToQuaternion(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
    DirectX::XMVECTOR qRight = DirectX::XMLoadFloat4(&right);
    DirectX::XMVECTOR qLeft = DirectX::XMLoadFloat4(&left);

    // 合成
    qRight = DirectX::XMQuaternionMultiply(qRight, q);
    qLeft = DirectX::XMQuaternionMultiply(qLeft, q);

    DirectX::XMFLOAT4 rotRight;
    DirectX::XMFLOAT4 rotLeft;
    DirectX::XMStoreFloat4(&rotRight, qRight);
    DirectX::XMStoreFloat4(&rotLeft, qLeft);

    m_boxBody.SetRotation(m_rotation);
    m_boxRight.SetRotation(rotRight);
    m_boxLeft.SetRotation(rotLeft);
    
    m_wingTimer += 0.15f;
    DirectX::XMFLOAT4 pivotRight = EulerToQuaternion(DirectX::XMFLOAT3(0.f, 0.f, 15 * cos(m_wingTimer)));
    DirectX::XMFLOAT4 pivotLeft = EulerToQuaternion(DirectX::XMFLOAT3(0.f, 0.f, -15 * cos(m_wingTimer)));
    m_boxRight.SetPivotRotation(pivotRight);
    m_boxLeft.SetPivotRotation(pivotLeft);
}

void Enemy::TakeDamage(int amount)
{
	m_health -= amount;
    if (m_health < 0)
    {
        m_health = 0;
    }
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

void Enemy::setTargetPosRandom() 
{
    float r = (float)rand() / RAND_MAX;
    float minX = -5.f;
    float maxX = 5.f;
    float x = minX + r * (maxX - minX);

    r = (float)rand() / RAND_MAX;
    float minY = 0.75f;
    float maxY = 1.5f;
    float y = minY + r * (maxY - minY);

    r = (float)rand() / RAND_MAX;
    float minZ = -5.f;
    float maxZ = 5.f;
    float z = minZ + r * (maxZ - minZ);
    m_targetPos = { x,y,z };
}

void Enemy::Attack() 
{
    m_isAttack = true;
}