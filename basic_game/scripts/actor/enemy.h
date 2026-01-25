#pragma once
#include "actor.h"
#include "../model/box_model.h"
#include "collider.h"

class Player;

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
	const Collider& const GetCollider() { return m_collider; }

private:
	void move();
	void look();
	void calcMoveAxis();
	void calcBoxPos();
	void calcBoxRot();

	BoxModel m_boxBody;
	BoxModel m_boxRight;
	BoxModel m_boxLeft;
	Player* m_pPlayer = nullptr;
	Collider m_collider;

	DirectX::XMFLOAT3 m_position = { 0, 0, -5.f };
	DirectX::XMFLOAT4 m_rotation = { 0, 0, 0, 0 };

	DirectX::XMFLOAT3 m_forward = { 0, 0, 1 };
	DirectX::XMFLOAT3 m_right = { 1, 0, 0 };

	float m_yaw;
	float m_pitch;
};