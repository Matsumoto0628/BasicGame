#pragma once
#include "camera.h"
#include "stage.h"
#include "weapon.h"

class Renderer;

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void Initialize(Renderer& renderer);
    void Terminate();

    void Update();
    void Draw();

private:
    Renderer* m_pRenderer = nullptr;
    Camera m_camera;
    Stage m_stage;
    Weapon m_weapon;
};