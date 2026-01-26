#pragma once
#include "actor.h"
#include "../model/box_model.h"
#include "collider.h"

class Player;

enum MoveType
{
	Curve,
	Basic,
};

class Enemy : public Actor
{
public:
	Enemy();
	~Enemy();
	void Initialize(Renderer &renderer, Player* pPlayer);
	void Setup() override;
	void Update() override;
	void Draw() override;
	void Terminate() override;
	const Collider& GetCollider() const { return m_collider; }
	void TakeDamage(int amount);
	void SetPosition(const DirectX::XMFLOAT3& pos) { m_position = pos; }
	void SetMoveType(MoveType type) { m_moveType = type; }
	void Attack();
	bool IsDead() const { return m_isDead; }
	void CancelAttack();

private:
	void move();
	void look();
	void calcMoveAxis();
	void calcBoxPos();
	void calcBoxRot();
	float lerpAngle(float a, float b, float t);
	void knockback();
	void curveLook();
	void basicLook();
	void setTargetPosRandom();

	BoxModel m_boxBody;
	BoxModel m_boxRight;
	BoxModel m_boxLeft;
	Player* m_pPlayer = nullptr;
	Collider m_collider;
	Renderer* m_pRenderer = nullptr;

	DirectX::XMFLOAT3 m_position = { 0, 0, -5.f };
	DirectX::XMFLOAT4 m_rotation = { 0, 0, 0, 0 };

	DirectX::XMFLOAT3 m_forward = { 0, 0, 1 };
	DirectX::XMFLOAT3 m_right = { 1, 0, 0 };

	float m_yaw;
	float m_pitch;

	int m_health = 30;

	bool m_isHit = false;
	bool m_isKnockback = false;
	float m_hitTimer = 0.f;
	float m_knockbackTimer = 0.f;
	static constexpr float HIT_DURATION = 0.25f;
	static constexpr float KNOCKBACK_DURATION = 0.5f;

	float m_wingTimer = 0.f;
	float m_waveTimer = 0.f;

	MoveType m_moveType = MoveType::Basic;

	float m_sideYawOffset = 0.f;
	float m_dirChangeTimer = 0.f;

	DirectX::XMFLOAT3 m_targetPos{ 0,0,0 };

	float m_targetTimer = 0.f;
	float m_targetDuration = 3.f;

	bool m_isAttack = false;
	float m_attackTimer = 0.f;

	bool m_isDead = false;
	float m_deadTimer = 0.f;
};