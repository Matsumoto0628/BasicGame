#pragma once
#include "actor.h"

class Camera;
class Weapon;

class Player : public Actor 
{
public:
	Player();
	~Player();
	void Initialize(Camera* pCamera, Weapon* pWeapon);
	void Setup() override;
	void Update() override;
	void Draw() override;
	void Terminate() override;
	DirectX::XMFLOAT3 GetPosition() { return m_position; }

private:
	void move();
	void look();
	void calcMoveAxis();
	void calcWeaponPos();
	void calcWeaponRot();

	Camera* m_pCamera = nullptr;
	Weapon* m_pWeapon = nullptr;

	DirectX::XMFLOAT3 m_position = { 0, 0, -5.f };
	DirectX::XMFLOAT4 m_rotation = { 0, 0, 0, 0 };

	DirectX::XMFLOAT3 m_forward = { 0, 0, 1 };
	DirectX::XMFLOAT3 m_right = { 1, 0, 0 };

	float m_yaw;
	float m_pitch;
};