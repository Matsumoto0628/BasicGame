#pragma once
#include "actor.h"
#include "../model/weapon_model.h"

class Renderer;

class Weapon : public Actor 
{
public:
	Weapon();
	~Weapon();
	void Initialize(Renderer& renderer);
	void Setup() override;
	void Update() override;
	void Draw() override;
	void Terminate() override;
	void Slash();
	void SetPosition(const DirectX::XMFLOAT3& pos);
	void SetRotation(const DirectX::XMFLOAT4& rot);
	void SetScale(const DirectX::XMFLOAT3& scale);
	void SetPivot(const DirectX::XMFLOAT3& pivot);
	void SetPivotRotation(const DirectX::XMFLOAT4& rot);

private:
	bool m_isAnimation = false;
	float m_animationTime = 0.f;
	static constexpr float ANIMATION_DURATION = 0.5f;
	WeaponModel m_weaponModel;
};