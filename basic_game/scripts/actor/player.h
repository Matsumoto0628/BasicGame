#pragma once
#include "camera.h"

class Player 
{
public:
	Player();
	~Player();
	void Initialize(Camera* pCamera);
	void Update();
	void SetPosition(const DirectX::XMFLOAT3& pos);
	void SetRotation(const DirectX::XMFLOAT3& rot); // pitch, yaw, roll
	void AddPosition(const DirectX::XMFLOAT3& dir, float amount);
	void AddRotation(const DirectX::XMFLOAT3& dir, float amount); // pitch, yaw, roll

private:
	void move();
	void look();
	void calcMoveAxis();

	Camera* m_pCamera = nullptr;
	DirectX::XMFLOAT3 m_position = { 0, 0, -5.f };
	DirectX::XMFLOAT3 m_rotation = { 0, 0, 0 }; // x=pitch y=yaw z=roll

	DirectX::XMFLOAT3 m_forward = { 0, 0, 1 };
	DirectX::XMFLOAT3 m_right = { 1, 0, 0 };
};