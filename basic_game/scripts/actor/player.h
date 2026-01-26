#pragma once
#include "actor.h"
#include "collider.h"

class Camera;
class Weapon;
class Renderer;

class Player : public Actor 
{
public:
	Player();
	~Player();
	void Initialize(Renderer& renderer, Camera* pCamera, Weapon* pWeapon);
	void Setup() override;
	void Update() override;
	void Draw() override;
	void Terminate() override;
	DirectX::XMFLOAT3 GetPosition() { return m_position; }
	void TakeDamage(int amount);
	const Collider& GetCollider() const { return m_collider; }

private:
	void move();
	void look();
	void calcMoveAxis();
	void calcWeaponPos();
	void calcWeaponRot();
	void knockback();

	Camera* m_pCamera = nullptr;
	Weapon* m_pWeapon = nullptr;
	Collider m_collider;

	DirectX::XMFLOAT3 m_position = { 0, 0, -5.f };
	DirectX::XMFLOAT4 m_rotation = { 0, 0, 0, 0 };

	DirectX::XMFLOAT3 m_forward = { 0, 0, 1 };
	DirectX::XMFLOAT3 m_right = { 1, 0, 0 };

	float m_yaw;
	float m_pitch;

	float m_health = 100;
	
	bool m_isKnockback = false;
	float m_knockbackTimer = 0.f;
	static constexpr float KNOCKBACK_DURATION = 0.25f;

	bool m_isHit = false;
	float m_hitTimer = 0.f;
	static constexpr float HIT_DURATION = 2.f;
};