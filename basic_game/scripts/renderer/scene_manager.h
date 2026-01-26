#pragma once
#include "camera.h"
#include "stage_model.h"
#include "weapon.h"
#include "player.h"
#include "enemy.h"

class Renderer;
class Renderer2D;

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void Initialize(Renderer& renderer, Renderer2D& renderer2d);
    void Setup();
    void Terminate();

    void Update();
    void Draw();

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