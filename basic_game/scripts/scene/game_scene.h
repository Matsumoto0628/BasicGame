#pragma once
#include "scene.h"
#include "camera.h"
#include "stage_model.h"
#include "weapon.h"
#include "player.h"
#include "enemy.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();
	void Initialize(Renderer& renderer, Renderer2D& renderer2d) override;
	void Setup() override;
	void Update() override;
	void Draw() override;
	void Terminate() override;
private:
	Renderer* m_pRenderer = nullptr;
	Renderer2D* m_pRenderer2d = nullptr;
	Camera m_camera;
	StageModel m_stage;
	Weapon m_weapon;
	Player m_player;

	static constexpr int ENEMY_MAX = 12;
	Enemy m_enemies[ENEMY_MAX];
};