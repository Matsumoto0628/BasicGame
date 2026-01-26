#pragma once
#include "camera.h"
#include "stage_model.h"
#include "weapon.h"
#include "player.h"
#include "enemy.h"

class Renderer;

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void Initialize(Renderer& renderer);
    void Setup();
    void Terminate();

    void Update();
    void Draw();

private:
    Renderer* m_pRenderer = nullptr;
    Camera m_camera;
    StageModel m_stage;
    Weapon m_weapon;
	Player m_player;

	static constexpr int ENEMY_MAX = 12;
	Enemy m_enemies[ENEMY_MAX];
};