#include "weapon.h"
#include "euler_converter.h"
#include "renderer.h"

Weapon::Weapon()
{
}

Weapon::~Weapon()
{
}

void Weapon::Initialize(Renderer &renderer) 
{
	m_pRenderer = &renderer;

	const char* WEAPON_PATH = "models/weapon/weapon.obj";
	m_weaponModel.Setup(renderer, WEAPON_PATH);
	
	m_collider.Initialize(renderer);
}

void Weapon::Setup()
{
	SetPivot({ 0, -0.1f, 0 });
	SetScale({ 0.1f, 0.1f, 0.1f });
	m_collider.SetRadius(0.1f);
}

void Weapon::Update()
{
	m_collider.Update();
	m_collider.SetPosition(m_attackPos);

	if (m_isAnimation)
	{
		if (m_animationTime >= ANIMATION_DURATION)
		{
			m_isAnimation = false;
			m_animationTime = 0.f;
			SetPivotRotation(EulerToQuaternion(DirectX::XMFLOAT3(0, 0, 0)));
			m_collider.SetActive(false);
			return;
		}

		m_animationTime += 0.017f;
		float timeDecay = 1 - m_animationTime / ANIMATION_DURATION;
		SetPivotRotation(EulerToQuaternion(DirectX::XMFLOAT3(400.f * m_animationTime * timeDecay, 0, 0.f)));
	}
}

void Weapon::Draw()
{
	m_weaponModel.Draw();
	m_collider.Draw();
}

void Weapon::Terminate()
{
	m_weaponModel.Terminate();
}

void Weapon::Slash()
{
	m_isAnimation = true;
	m_collider.SetActive(true);
}

void Weapon::SetAttackPos(const DirectX::XMFLOAT3& pos)
{
	m_attackPos = pos;
}

void Weapon::SetPosition(const DirectX::XMFLOAT3& pos) 
{
	m_weaponModel.SetPosition(pos);
}

void Weapon::SetRotation(const DirectX::XMFLOAT4& rot) 
{
	m_weaponModel.SetRotation(rot);
}

void Weapon::SetScale(const DirectX::XMFLOAT3& scale)
{
	m_weaponModel.SetScale(scale);
}

void Weapon::SetPivot(const DirectX::XMFLOAT3& pivot) 
{
	m_weaponModel.SetPivot(pivot);
}

void Weapon::SetPivotRotation(const DirectX::XMFLOAT4& rot) 
{
	m_weaponModel.SetPivotRotation(rot);
}